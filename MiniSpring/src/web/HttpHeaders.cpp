#include "HttpHeaders.h"

void HttpHeaders::set(const std::string &name, const std::string &value) {
  m_headers[name] = value;
}

std::string HttpHeaders::get(const std::string &name) const {
  auto it = m_headers.find(name);

  if (it == m_headers.end()) return "";

  return it->second;
}

bool HttpHeaders::contains(const std::string &name) const {
  return m_headers.find(name) != m_headers.end();
}

const std::unordered_map<std::string, std::string> &HttpHeaders::all() const {
  return m_headers;
}