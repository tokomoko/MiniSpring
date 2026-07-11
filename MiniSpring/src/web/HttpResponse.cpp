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
	header("Content-Length", std::to_string(m_responseBody.size()));

	return response.str();
}

void HttpResponse::json(const JsonValue& value) {
	std::string text = value.stringify();

	std::cout << "JSON RESPONSE:\n";
	std::cout << text << std::endl;

	header("Content-Type", "application/json");
	body(text);
}

void HttpResponse::html(const std::string& content) {
	header("Content-Type", "text/html");
	body(content);
}