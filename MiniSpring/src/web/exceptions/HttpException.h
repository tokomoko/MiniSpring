#pragma once

#include <exception>
#include <string>

class HttpException : public std::exception {
public:
	HttpException(int status, std::string message);

	int status() const;
	const char* what() const noexcept override;

private:

	int m_status;
	std::string m_message;
};