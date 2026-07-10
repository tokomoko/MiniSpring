#include "HttpRequest.h"

#include <sstream>

HttpRequest::HttpRequest(const std::string& raw, JsonMapper& mapper) : m_mapper(mapper) {
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
			size_t valueStart = colon + 1;
			while (valueStart < line.size() && line[valueStart] == ' ') valueStart++;
			std::string value = line.substr(valueStart);

			if (!value.empty() && value.back() == '\r')
				value.pop_back();

			m_headers.set(name, value);
		}
	}

	std::ostringstream body;
	body << stream.rdbuf();

	m_body = body.str();
}

std::string HttpRequest::method() {
	return m_requestMethod;
}


std::string HttpRequest::path() {
	return m_requestPath;
}

void HttpRequest::setPathParameter(const std::string& name, const std::string& value) {
	m_pathParameters[name] = value;
}

std::string HttpRequest::pathParameter(const std::string& name) const {
	auto it = m_pathParameters.find(name);
	if (it == m_pathParameters.end()) return "";
	return it->second;
}