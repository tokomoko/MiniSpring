#include "HttpError.h"

HttpError::HttpError(int status, std::string message) : m_status(status), m_message(std::move(message)) {}

HttpError::HttpError(int status, std::string message, Json details) : m_status(status), m_message(std::move(message)), m_details(std::move(details)) {}

int HttpError::status() const {
	return m_status;
}

const std::string& HttpError::message() const {
	return m_message;
}

Json HttpError::toJson() const {
	Json json;

	json["status"] = m_status;
	json["message"] = m_message;
	json["details"] = m_details.asObject();

	return json;
}