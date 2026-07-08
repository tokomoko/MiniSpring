#include"HttpException.h"

HttpException::HttpException(int status, std::string message) : m_status(status), m_message(message) {}

int HttpException::status() const {
	return m_status;
}

const char* HttpException::what() const noexcept {
	return m_message.c_str();
}