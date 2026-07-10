#pragma once

#include"../core/Core.h"
#include"../core/IRequestHandler.h"

#include"../json/JsonMapper.h"

#include<winsock2.h>

class MINISPRING_API HttpServer {
public:
	HttpServer(IRequestHandler& r, JsonMapper& mapper);
	~HttpServer();

	void listen(int port);

private:
	SOCKET m_serverSocket;
	IRequestHandler* m_reqHandler;

	JsonMapper m_mapper;
};