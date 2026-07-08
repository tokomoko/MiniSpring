#include "HttpResponse.h"

#include<sstream>

HttpResponse::HttpResponse() {
	m_statusCode = 200;
	m_statusMessage = "OK";
}

void HttpResponse::status(int code) {
	m_statusCode = code;
	m_statusMessage = statusText(code);
}

std::string HttpResponse::statusText(int code) {
	switch (code) {
	case 200: return "OK";
	case 201: return "Created";
	case 400: return "Bad Request";
	case 404: return "Not Found";
	case 500: return "Internal Server Error";
	default: return "Unknown";
	}
}

void HttpResponse::header(const std::string& name, const std::string& value) {
	m_headers.set(name, value);
}

void HttpResponse::body(const std::string& content) {
	m_responseBody = content;
}

std::string HttpResponse::build() {
	std::stringstream response;

	response << "HTTP/1.1 " << m_statusCode << " " << m_statusMessage << "\r\n";
	for (auto& header : m_headers.all())
		response << header.first << ": " << header.second << "\r\n";

	response << "\r\n";
	response << m_responseBody;

	return response.str();
}

void HttpResponse::json(Json json) {
	header("Content-Type", "application/json");
	body(json.stringify());
}

void HttpResponse::html(const std::string& content) {
	status(200);
	header("Content-Type", "text/html");
	body(content);
}