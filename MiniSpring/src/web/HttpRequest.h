#pragma once

#include"HttpHeaders.h"

#include <string>

class HttpRequest {
public:
	HttpRequest(const std::string& raw);

	std::string method();
	std::string path();
	std::string header(const std::string& name);

private:
	std::string m_requestMethod;
	std::string m_requestPath;

	HttpHeaders m_headers;
};