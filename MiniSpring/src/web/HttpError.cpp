#include "HttpError.h"

#include "../json/JsonValue.h"

HttpError::HttpError(int status, std::string message)
    : m_status(status), m_message(std::move(message)) {}

HttpError::HttpError(int status, std::string message, JsonValue details)
    : m_status(status),
      m_message(std::move(message)),
      m_details(std::move(details)) {}

int HttpError::status() const { return m_status; }

const std::string &HttpError::message() const { return m_message; }

JsonValue HttpError::toJson() const {
  JsonObject object;
  object["status"] = m_status;
  object["message"] = m_message;
  object["details"] = m_details;

  return JsonValue(object);
}