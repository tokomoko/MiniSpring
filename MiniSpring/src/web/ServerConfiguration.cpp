#include "ServerConfiguration.h"

ServerConfiguration &ServerConfiguration::port(int _port) {
  m_port = _port;
  return *this;
}

ServerConfiguration &ServerConfiguration::address(const std::string &_address) {
  m_address = _address;
  return *this;
}

ServerConfiguration &ServerConfiguration::threads(int count) {
  m_threads = count;
  return *this;
}

ServerConfiguration &ServerConfiguration::backlog(int _backlog) {
  m_backlog = _backlog;
  return *this;
}

ServerConfiguration &ServerConfiguration::queueSize(size_t _queueSize) {
  m_queueSize = _queueSize;
  return *this;
}

ServerConfiguration &ServerConfiguration::keepAlive(bool _enabled) {
  m_keepAlive = _enabled;
  return *this;
}

ServerConfiguration &ServerConfiguration::keepAliveTimeout(
    std::chrono::milliseconds _keepAliveTimeout) {
  m_keepAliveTimeout = _keepAliveTimeout;
  return *this;
}

ServerConfiguration &ServerConfiguration::keepAliveRequests(
    size_t _keepAliveRequests) {
  m_keepAliveRequests = _keepAliveRequests;
  return *this;
}

ServerConfiguration &ServerConfiguration::requestTimeout(
    std::chrono::milliseconds _requestTimeout) {
  m_requestTimeout = _requestTimeout;
  return *this;
}

ServerConfiguration &ServerConfiguration::sendTimeout(
    std::chrono::milliseconds _sendTimeout) {
  m_sendTimeout = _sendTimeout;
  return *this;
}

ServerConfiguration &ServerConfiguration::maxRequestSize(size_t _bytes) {
  m_maxRequestSize = _bytes;
  return *this;
}

ServerConfiguration &ServerConfiguration::maxHeaderSize(size_t _maxHeaderSize) {
  m_maxHeaderSize = _maxHeaderSize;
  return *this;
}

ServerConfiguration &ServerConfiguration::maxHeaderCount(
    size_t _maxHeaderCount) {
  m_maxHeaderCount = _maxHeaderCount;
  return *this;
}

ServerConfiguration &ServerConfiguration::maxUrlLength(size_t _maxUrlLength) {
  m_maxUrlLength = _maxUrlLength;
  return *this;
}

int ServerConfiguration::port() const { return m_port; }

const std::string &ServerConfiguration::address() const { return m_address; }

int ServerConfiguration::threads() const { return m_threads; }

int ServerConfiguration::backlog() const { return m_backlog; }

size_t ServerConfiguration::queueSize() const { return m_queueSize; }

bool ServerConfiguration::keepAlive() const { return m_keepAlive; }

std::chrono::milliseconds ServerConfiguration::keepAliveTimeout() const {
  return m_keepAliveTimeout;
}

size_t ServerConfiguration::keepAliveRequests() const {
  return m_keepAliveRequests;
}

std::chrono::milliseconds ServerConfiguration::requestTimeout() const {
  return m_requestTimeout;
}

std::chrono::milliseconds ServerConfiguration::sendTimeout() const {
  return m_sendTimeout;
}

size_t ServerConfiguration::maxRequestSize() const { return m_maxRequestSize; }

size_t ServerConfiguration::maxHeaderSize() const { return m_maxHeaderSize; }

size_t ServerConfiguration::maxHeaderCount() const { return m_maxHeaderCount; }

size_t ServerConfiguration::maxUrlLength() const { return m_maxUrlLength; }