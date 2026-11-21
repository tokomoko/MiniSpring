#pragma once

#include "../core/Core.h"

#include "../json/JsonValue.h"

#include <string>

class MINISPRING_API HttpError {
 public:
  HttpError(int status, std::string message);
  HttpError(int status, std::string message, JsonValue details);

  int status() const;
  const std::string &message() const;
  JsonValue toJson() const;

 private:
  int m_status;
  std::string m_message;
  JsonValue m_details;
};