#pragma once

#include"../core/Core.h"

class MINISPRING_API ServerConfiguration {
public:
	ServerConfiguration& port(int port);
	ServerConfiguration& threads(int count);
	ServerConfiguration& keepAlive(bool enabled);

	int port() const;
	int threads() const;
	bool keepAlive() const;

private:
	int m_port = 8080;
	int m_threads = 1;
	bool m_keepAlive = true;
};