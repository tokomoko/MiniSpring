#pragma once

#include<winsock2.h>

#include"../core/IRequestHandler.h"

class HttpServer {
public:
	HttpServer(IRequestHandler& r);
	~HttpServer();

	void listen(int port);

private:
	SOCKET m_serverSocket;
	IRequestHandler* m_reqHandler;
};