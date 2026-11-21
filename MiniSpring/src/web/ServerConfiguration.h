#pragma once

#include "../core/Core.h"

#include <chrono>
#include <string>
#include <thread>

class MINISPRING_API ServerConfiguration {
 public:
  ServerConfiguration &port(int _port);
  ServerConfiguration &address(const std::string &_address);
  ServerConfiguration &threads(int _threads);
  ServerConfiguration &backlog(int _backlog);
  ServerConfiguration &queueSize(size_t _queueSize);
  ServerConfiguration &keepAlive(bool _keepAlive);
  ServerConfiguration &keepAliveTimeout(
      std::chrono::milliseconds _keepAliveTimeout);
  ServerConfiguration &keepAliveRequests(size_t _keepAliveRequests);
  ServerConfiguration &requestTimeout(
      std::chrono::milliseconds _requestTimeout);
  ServerConfiguration &sendTimeout(std::chrono::milliseconds _sendTimeout);
  ServerConfiguration &maxRequestSize(size_t _maxRequestSize);
  ServerConfiguration &maxHeaderSize(size_t _maxHeaderSize);
  ServerConfiguration &maxHeaderCount(size_t _maxHeaderCount);
  ServerConfiguration &maxUrlLength(size_t _maxUrlLength);

  int port() const;
  const std::string &address() const;
  int threads() const;
  int backlog() const;
  size_t queueSize() const;
  bool keepAlive() const;
  std::chrono::milliseconds keepAliveTimeout() const;
  size_t keepAliveRequests() const;
  std::chrono::milliseconds requestTimeout() const;
  std::chrono::milliseconds sendTimeout() const;
  size_t maxRequestSize() const;
  size_t maxHeaderSize() const;
  size_t maxHeaderCount() const;
  size_t maxUrlLength() const;

 private:
  std::string m_address = "0.0.0.0";

  int m_port = 8080;
  int m_threads = std::thread::hardware_concurrency();
  int m_backlog = 0;

  std::chrono::milliseconds m_keepAliveTimeout{30000};
  std::chrono::milliseconds m_requestTimeout{15000};
  std::chrono::milliseconds m_sendTimeout{15000};

  size_t m_queueSize = 1024;
  size_t m_keepAliveRequests = 100;
  size_t m_maxRequestSize = 8 * 1024 * 1024;
  size_t m_maxHeaderSize = 16 * 1024;
  size_t m_maxHeaderCount = 100;
  size_t m_maxUrlLength = 2048;

  bool m_keepAlive = true;
};