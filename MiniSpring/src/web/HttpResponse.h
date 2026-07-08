#pragma once

#include"HttpHeaders.h"
#include"../json/Json.h"

#include<string>

class HttpResponse {
public:
	HttpResponse();

	void send(const std::string& body);
	void status(int code);

	void header(const std::string& name, const std::string& value);
	void body(const std::string& content);
	void json(Json json);
	void html(const std::string& content);

	std::string build();

private:
	std::string statusText(int code);

private:
	int m_statusCode;
	std::string m_statusMessage;
	std::string m_responseBody;
	HttpHeaders m_headers;
};