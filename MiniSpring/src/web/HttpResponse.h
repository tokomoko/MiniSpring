#pragma once

#include "../core/Core.h"

#include "../json/JsonValue.h"

#include "HttpHeaders.h"

#include <string>

class MINISPRING_API HttpResponse {
 public:
  HttpResponse();

  static HttpResponse parse(const std::string &raw);

  void status(int code);
  int statusCode() const { return m_statusCode; }

  void setCookie(const std::string &name, const std::string &value,
                 bool httpOnly = true, const std::string &sameSite = "Strict",
                 int maxAgeSeconds = -1);
  void clearCookie(const std::string &name);

  void header(const std::string &name, const std::string &value);
  std::string header(const std::string &name) const {
    return m_headers.get(name);
  }

  void body(const std::string &content);
  const std::string &body() const { return m_responseBody; }

  void json(const JsonValue &json);
  void html(const std::string &content);

  std::string build();

 private:
  std::string statusText(int code);

 private:
  int m_statusCode;
  std::string m_statusMessage;
  std::string m_responseBody;
  HttpHeaders m_headers;
};