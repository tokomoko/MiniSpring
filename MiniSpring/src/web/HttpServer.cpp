#include"HttpServer.h"

#include"HttpRequest.h"
#include"HttpResponse.h"

#include<iostream>

#pragma comment(lib,"ws2_32.lib")

HttpServer::HttpServer(IRequestHandler& r) : m_reqHandler(&r) {
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

	if (result == SOCKET_ERROR)
		throw std::runtime_error("Bind failed");

	::listen(m_serverSocket, SOMAXCONN);

	std::cout << "MiniSpring running on port " << port << std::endl;

	while (true) {
		SOCKET client = accept(m_serverSocket, nullptr, nullptr);

		char buffer[4096];
		int size = recv(client, buffer, sizeof(buffer), 0);
		std::string raw(buffer, size);

		HttpRequest request(raw);
		std::cout << "[REQUEST] " << request.method() << " " << request.path() << std::endl;

		HttpResponse response;
		bool found = m_reqHandler->handle(request, response);

		if (!found) {
			response.status(404);
			response.body("Not Found");
		}

		std::string data = response.build();
		std::cout << "[RESPONSE] " << data << std::endl;

		send(client, data.c_str(), data.size(), 0);

		closesocket(client);
	}
}