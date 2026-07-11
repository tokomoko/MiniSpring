#include"HttpServer.h"

#include"HttpRequest.h"
#include"HttpResponse.h"

#include<iostream>
#include<sstream>

#pragma comment(lib,"ws2_32.lib")

HttpServer::HttpServer(IRequestHandler& r, JsonMapper& mapper) : m_reqHandler(&r), m_mapper(mapper) {
	WSADATA data;
	int result = WSAStartup(MAKEWORD(2, 2), &data);

	if (result != 0)
		throw std::runtime_error("WSAStartup failed");


	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_serverSocket == INVALID_SOCKET)
		throw std::runtime_error("Socket creation failed");
}

HttpServer::~HttpServer() {
	closesocket(m_serverSocket);
	WSACleanup();
}

void HttpServer::listen(int port) {
	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	int result = bind(m_serverSocket, (sockaddr*)&address, sizeof(address));

	if (result == SOCKET_ERROR) throw std::runtime_error("Bind failed");

	::listen(m_serverSocket, SOMAXCONN);

	while (true) {
		SOCKET client = accept(m_serverSocket, nullptr, nullptr);

		std::string raw = receiveRequest(client);

		HttpRequest request(raw, m_mapper);
		std::cout << "[REQUEST] " << request.method() << " " << request.path() << std::endl;

		HttpResponse response;
		bool found = m_reqHandler->handle(request, response);

		if (!found) {
			response.status(404);
			response.body("Not Found");
		}

		std::string data = response.build();
		std::cout << "[RESPONSE] " << data << std::endl;

		sendAll(client, data);

		closesocket(client);
	}
}

bool HttpServer::sendAll(SOCKET socket, const std::string& data) {
	size_t totalSent = 0;

	while (totalSent < data.size())
	{
		int sent = send(socket, data.c_str() + totalSent, static_cast<int>(data.size() - totalSent), 0);
		if (sent == SOCKET_ERROR) return false;
		totalSent += sent;
	}

	return true;
}

std::string HttpServer::receiveRequest(SOCKET client)
{
	std::string request;

	char buffer[4096];


	while (true)
	{
		int received = recv(
			client,
			buffer,
			sizeof(buffer),
			0
		);


		if (received <= 0)
			break;


		request.append(buffer, received);


		if (received < sizeof(buffer))
			break;
	}


	return request;
}