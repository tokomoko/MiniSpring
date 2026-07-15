#pragma once

#include"../../core/Core.h"

#include <exception>
#include <string>

class HttpException : public std::exception {
public:
	HttpException(int status, std::string message) : m_status(status), m_message(message) {}

	int status() const {
		return m_status;
	}

	const char* what() const noexcept override {
		return m_message.c_str();
	}

private:
	int m_status;
	std::string m_message;
};