#include"ServerConfiguration.h"

ServerConfiguration& ServerConfiguration::port(int port) {
	m_port = port;
	return *this;
}

ServerConfiguration& ServerConfiguration::threads(int count) {
	m_threads = count;
	return *this;
}

ServerConfiguration& ServerConfiguration::keepAlive(bool enabled) {
	m_keepAlive = enabled;
	return *this;
}

int ServerConfiguration::port() const {
	return m_port;
}

int ServerConfiguration::threads() const {
	return m_threads;
}

bool ServerConfiguration::keepAlive() const {
	return m_keepAlive;
}
