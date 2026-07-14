#pragma once

#include"../core/Core.h"
#include"../core/IRequestHandler.h"

#include"../json/JsonMapper.h"

#include"RequestQueue.h"
#include"ServerConfiguration.h"

#include<winsock2.h>
#include<thread>
#include<vector>
#include<atomic>

class MINISPRING_API HttpServer {
public:
	HttpServer(IRequestHandler& r, JsonMapper& mapper, const ServerConfiguration& config);
	~HttpServer();

	void listen(int port);
	void stop();

private:
	void acceptLoop();
	void workerLoop();
	void handleClient(SOCKET client);
	bool shouldKeepAlive(HttpRequest& request);

	bool sendAll(SOCKET socket, const std::string& data);
	std::string receiveRequest(SOCKET client);
	static size_t parseContentLength(const std::string& headerBlock);

	void configureSocket(SOCKET socket);
	void checkRequestSize(size_t receivedBytes);
	void checkHeaderSize(size_t size);

private:
	SOCKET m_serverSocket;
	IRequestHandler* m_reqHandler;
	JsonMapper m_mapper;
	RequestQueue m_queue;

	std::vector<std::thread> m_workers;
	std::atomic<bool> m_running = false;

	const ServerConfiguration& m_config;
};