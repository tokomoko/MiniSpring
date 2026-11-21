#include "HttpRequest.h"

#include <sstream>

HttpRequest::HttpRequest(const std::string &raw, JsonMapper &mapper)
    : m_mapper(mapper) {
  std::stringstream stream(raw);
  std::string line;

  getline(stream, line);

  std::stringstream first(line);

  first >> m_requestMethod >> m_requestPath;

  size_t queryPos = m_requestPath.find('?');
  if (queryPos != std::string::npos) {
    m_queryString = m_requestPath.substr(queryPos + 1);
    m_requestPath = m_requestPath.substr(0, queryPos);
  }

  while (getline(stream, line)) {
    if (line == "\r") break;

    size_t colon = line.find(':');

    if (colon != std::string::npos) {
      std::string name = line.substr(0, colon);
      size_t valueStart = colon + 1;
      while (valueStart < line.size() && line[valueStart] == ' ') valueStart++;
      std::string value = line.substr(valueStart);

      if (!value.empty() && value.back() == '\r') value.pop_back();

      m_headers.set(name, value);
    }
  }

  std::ostringstream body;
  body << stream.rdbuf();

  m_body = body.str();
}

std::string HttpRequest::method() { return m_requestMethod; }

std::string HttpRequest::path() { return m_requestPath; }

std::string HttpRequest::cookie(const std::string &name) const {
  std::string header = m_headers.get("Cookie");
  size_t pos = 0;

  while (pos < header.size()) {
    size_t end = header.find(';', pos);
    if (end == std::string::npos) end = header.size();

    std::string pair = header.substr(pos, end - pos);
    size_t eq = pair.find('=');
    if (eq != std::string::npos) {
      size_t start = pair.find_first_not_of(' ');
      std::string key = pair.substr(start, eq - start);
      if (key == name) return pair.substr(eq + 1);
    }

    pos = end + 1;
  }

  return "";
}

void HttpRequest::setPathParameter(const std::string &name,
                                   const std::string &value) {
  m_pathParameters[name] = value;
}

std::string HttpRequest::pathParameter(const std::string &name) const {
  auto it = m_pathParameters.find(name);
  if (it == m_pathParameters.end()) return "";
  return it->second;
}