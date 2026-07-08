#pragma once

#include "../json/Json.h"

#include <string>

class HttpError {
public:
	HttpError(int status, std::string message);
	HttpError(int status, std::string message, Json details);

	int status() const;
	const std::string& message() const;
	Json toJson() const;

private:
	int m_status;
	std::string m_message;
	Json m_details;
};