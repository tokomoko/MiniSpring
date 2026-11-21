#include "HttpResponse.h"

#include <sstream>

HttpResponse::HttpResponse() {
  m_statusCode = 200;
  m_statusMessage = "OK";
}

HttpResponse HttpResponse::parse(const std::string &raw) {
  HttpResponse res;

  size_t headerEnd = raw.find("\r\n\r\n");
  if (headerEnd == std::string::npos) {
    res.status(500);
    res.body("Invalid HTTP Response");
    return res;
  }

  std::string headerBlock = raw.substr(0, headerEnd);
  res.m_responseBody = raw.substr(headerEnd + 4);

  std::stringstream stream(headerBlock);
  std::string statusLine;
  if (std::getline(stream, statusLine)) {
    std::stringstream lineStream(statusLine);
    std::string httpVer;
    int code = 200;
    lineStream >> httpVer >> code;
    res.status(code);
  }

  std::string line;
  while (std::getline(stream, line)) {
    if (line == "\r" || line.empty()) continue;

    size_t colon = line.find(':');
    if (colon != std::string::npos) {
      std::string name = line.substr(0, colon);
      size_t valueStart = colon + 1;
      while (valueStart < line.size() && line[valueStart] == ' ') valueStart++;
      std::string value = line.substr(valueStart);

      if (!value.empty() && value.back() == '\r') value.pop_back();

      res.header(name, value);
    }
  }

  return res;
}

void HttpResponse::status(int code) {
  m_statusCode = code;
  m_statusMessage = statusText(code);
}

std::string HttpResponse::statusText(int code) {
  switch (code) {
    case 200:
      return "OK";
    case 201:
      return "Created";
    case 204:
      return "No Content";
    case 400:
      return "Bad Request";
    case 401:
      return "Unauthorized";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 500:
      return "Internal Server Error";
    default:
      return "Unknown";
  }
}

void HttpResponse::header(const std::string &name, const std::string &value) {
  m_headers.set(name, value);
}

void HttpResponse::body(const std::string &content) {
  m_responseBody = content;
}

void HttpResponse::setCookie(const std::string &name, const std::string &value,
                             bool httpOnly, const std::string &sameSite,
                             int maxAgeSeconds) {
  std::string cookie = name + "=" + value + "; Path=/; SameSite=" + sameSite;
  if (maxAgeSeconds >= 0)
    cookie += "; Max-Age=" + std::to_string(maxAgeSeconds);
  if (httpOnly) cookie += "; HttpOnly";
  header("Set-Cookie", cookie);
}

void HttpResponse::clearCookie(const std::string &name) {
  setCookie(name, "", true, "Strict", 0);
}

std::string HttpResponse::build() {
  header("Content-Length", std::to_string(m_responseBody.size()));  // set first

  std::stringstream response;

  response << "HTTP/1.1 " << m_statusCode << " " << m_statusMessage << "\r\n";
  for (auto &header : m_headers.all())
    response << header.first << ": " << header.second << "\r\n";

  response << "\r\n";
  response << m_responseBody;

  return response.str();
}

void HttpResponse::json(const JsonValue &value) {
  std::string text = value.stringify();
  header("Content-Type", "application/json");
  body(text);
}

void HttpResponse::html(const std::string &content) {
  header("Content-Type", "text/html");
  body(content);
}