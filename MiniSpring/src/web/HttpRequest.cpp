#include "HttpRequest.h"

#include <sstream>

HttpRequest::HttpRequest(const std::string& raw) {
	std::stringstream stream(raw);
	std::string line;

	getline(stream, line);

	std::stringstream first(line);

	first >> m_requestMethod >> m_requestPath;

	while (getline(stream, line)) {
		if (line == "\r")
			break;

		size_t colon = line.find(':');

		if (colon != std::string::npos) {
			std::string name = line.substr(0, colon);

			std::string value = line.substr(colon + 2);
			
			if (!value.empty() && value.back() == '\r')
				value.pop_back();

			m_headers.set(name, value);
		}
	}
}

std::string HttpRequest::method() {
	return m_requestMethod;
}


std::string HttpRequest::path() {
	return m_requestPath;
}