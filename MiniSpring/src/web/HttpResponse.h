#pragma once

#include"../core/Core.h"

#include"../json/JsonValue.h"

#include"HttpHeaders.h"

#include<string>

class MINISPRING_API HttpResponse {
public:
	HttpResponse();

	void status(int code);
	void setCookie(const std::string& name, const std::string& value, bool httpOnly = true, const std::string& sameSite = "Strict");

	void header(const std::string& name, const std::string& value);
	void body(const std::string& content);
	void json(const JsonValue& json);
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