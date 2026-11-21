#pragma once

#include "../core/Core.h"
#include "../orm/EntityManager.h"
#include "Authentication.h"
#include "Session.h"

#include <ctime>
#include <iomanip>
#include <optional>
#include <random>
#include <sstream>

class MINISPRING_API SessionManager {
 public:
  SessionManager(EntityManager &entityManager, int ttlSeconds = 86400)
      : m_entityManager(entityManager), m_ttlSeconds(ttlSeconds) {}

  std::string createSession(const Authentication &auth) {
    Session session;
    session.id = generateSessionId();
    session.username = auth.principal();
    session.authorities = join(auth.authorities());
    session.expiresAt = static_cast<int>(std::time(nullptr)) + m_ttlSeconds;

    m_entityManager.persist(session);
    return session.id;
  }

  std::optional<Authentication> getSession(const std::string &id) {
    auto session = m_entityManager.find<Session>(id);
    if (!session) return std::nullopt;

    if (session->expiresAt < static_cast<int>(std::time(nullptr))) {
      m_entityManager.removeById<Session>(Value(id));
      return std::nullopt;
    }

    return Authentication(session->username, split(session->authorities));
  }

  void destroySession(const std::string &id) {
    m_entityManager.removeById<Session>(Value(id));
  }

 private:
  static std::string generateSessionId() {
    std::random_device rd;
    std::mt19937_64 gen1(rd()), gen2(rd());
    std::uniform_int_distribution<uint64_t> dist;

    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(16) << dist(gen1)
       << std::setw(16) << dist(gen2);
    return ss.str();
  }

  static std::string join(const std::vector<std::string> &parts) {
    std::string out;
    for (size_t i = 0; i < parts.size(); i++) {
      if (i) out += ",";
      out += parts[i];
    }
    return out;
  }

  static std::vector<std::string> split(const std::string &text) {
    std::vector<std::string> parts;
    std::stringstream ss(text);
    std::string part;
    while (std::getline(ss, part, ','))
      if (!part.empty()) parts.push_back(part);
    return parts;
  }

 private:
  EntityManager &m_entityManager;
  int m_ttlSeconds;
};