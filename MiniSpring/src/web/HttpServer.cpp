#include"HttpServer.h"

#include"exceptions/HttpException.h"
#include"HttpRequest.h"
#include"HttpResponse.h"

#include<algorithm>
#include<cctype>

#pragma comment(lib,"ws2_32.lib")

HttpServer::HttpServer(IRequestHandler& r, JsonMapper& mapper, const ServerConfiguration& config) : m_reqHandler(&r), m_mapper(mapper), m_config(config) {
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
		throw std::runtime_error("WSAStartup failed");

	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
		throw std::runtime_error("Socket creation failed");
}

HttpServer::~HttpServer() {
	stop();
	closesocket(m_serverSocket);
	WSACleanup();
}

void HttpServer::listen(int port) {
	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(m_serverSocket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR)
		throw std::runtime_error("Bind failed");

	::listen(m_serverSocket, SOMAXCONN);

	m_running = true;

	m_workers.reserve(m_config.threads());
	for (int i = 0; i < m_config.threads(); i++)
		m_workers.emplace_back(&HttpServer::workerLoop, this);

	acceptLoop();
}

void HttpServer::stop() {
	if (!m_running.exchange(false))
		return;

	closesocket(m_serverSocket);
	m_queue.shutdown();

	for (auto& worker : m_workers)
		if (worker.joinable()) worker.join();

	m_workers.clear();
}

void HttpServer::acceptLoop() {
	while (m_running) {
		SOCKET client = accept(m_serverSocket, nullptr, nullptr);

		if (client == INVALID_SOCKET) {
			if (!m_running) break;
			continue;
		}

		configureSocket(client);
		m_queue.push(client);
	}
}

void HttpServer::workerLoop() {
	SOCKET client;
	while (m_queue.pop(client)) {
		handleClient(client);

	}
}

bool HttpServer::shouldKeepAlive(HttpRequest& request) {
	if (!m_config.keepAlive()) return false;

	std::string connection = request.header("Connection");
	std::transform(connection.begin(), connection.end(), connection.begin(), [](unsigned char c) {
		return std::tolower(c);
	});

	if (connection == "close") return false;
	return true;
}

void HttpServer::handleClient(SOCKET client) {
	bool keepOpen = true;
	size_t requestCount = 0;

	while (keepOpen) {
		std::string raw = receiveRequest(client);
		if (raw.empty()) break;

		HttpRequest request(raw, m_mapper);
		HttpResponse response;

		bool found;
		try {
			found = m_reqHandler->handle(request, response);
		}
		catch (const HttpException& e) {
			response = HttpResponse();
			response.status(e.status());
			response.body(e.what());
			found = true;
		}
		catch (const std::exception& e) {
			std::cerr << "[ERROR] Unhandled exception while handling request: " << e.what() << std::endl;
			response = HttpResponse();
			response.status(500);
			response.body("Internal Server Error");
			found = true;
		}
		catch (...) {
			std::cerr << "[ERROR] Unknown unhandled exception while handling request" << std::endl;
			response = HttpResponse();
			response.status(500);
			response.body("Internal Server Error");
			found = true;
		}

		if (!found) {
			response.status(404);
			response.body("Not Found");
		}

		keepOpen = shouldKeepAlive(request);
		response.header("Connection", keepOpen ? "keep-alive" : "close");

		std::string data = response.build();

		if (!sendAll(client, data)) break;

		requestCount++;
		if (requestCount >= m_config.keepAliveRequests())
			keepOpen = false;
	}

	closesocket(client);
}

bool HttpServer::sendAll(SOCKET socket, const std::string& data) {
	size_t totalSent = 0;

	while (totalSent < data.size()) {
		int sent = send(socket, data.c_str() + totalSent, static_cast<int>(data.size() - totalSent), 0);
		if (sent == SOCKET_ERROR) return false;
		totalSent += sent;
	}

	return true;
}

size_t HttpServer::parseContentLength(const std::string& headerBlock) {
	std::string lower = headerBlock;
	std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { 
		return std::tolower(c); 
	});

	size_t pos = lower.find("content-length:");
	if (pos == std::string::npos) return 0;

	pos += std::string("content-length:").size();
	while (pos < headerBlock.size() && headerBlock[pos] == ' ') pos++;

	size_t end = headerBlock.find("\r\n", pos);
	if (end == std::string::npos) end = headerBlock.size();

	try {
		return std::stoul(headerBlock.substr(pos, end - pos));
	}
	catch (...) {
		return 0;
	}
}

std::string HttpServer::receiveRequest(SOCKET client) {
	std::string request;
	char buffer[4096];
	size_t receivedBytes = 0;

	size_t headerEnd = std::string::npos;
	while (headerEnd == std::string::npos) {
		int received = recv(client, buffer, sizeof(buffer), 0);

		if (received <= 0) return "";
		
		receivedBytes += received;
		checkRequestSize(receivedBytes);

		request.append(buffer, received);
		checkHeaderSize(request.size());

		headerEnd = request.find("\r\n\r\n");
	}

	size_t bodyStart = headerEnd + 4;
	size_t contentLength = parseContentLength(request.substr(0, headerEnd));

	checkRequestSize(contentLength);

	while (request.size() - bodyStart < contentLength) {
		int received = recv(client, buffer, sizeof(buffer), 0);
		if (received <= 0) break;
		request.append(buffer, received);
	}

	return request;
}

void HttpServer::configureSocket(SOCKET socket) {
	DWORD recvTimeout = static_cast<DWORD>(m_config.requestTimeout().count());
	setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&recvTimeout), sizeof(recvTimeout));

	DWORD sendTimeout = static_cast<DWORD>(m_config.sendTimeout().count());
	setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&sendTimeout), sizeof(sendTimeout));
}

void HttpServer::checkRequestSize(size_t receivedBytes) {
	if (receivedBytes > m_config.maxRequestSize()) throw HttpException(413, "Payload Too Large");
}

void HttpServer::checkHeaderSize(size_t size) {
	if (size > m_config.maxHeaderSize()) throw HttpException(431, "Headers Too Large");
}