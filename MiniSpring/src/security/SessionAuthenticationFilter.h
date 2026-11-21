#pragma once

#include "../web/Filter.h"
#include "../web/HttpRequest.h"
#include "../web/HttpResponse.h"

#include "SecurityContext.h"
#include "SessionManager.h"

class SessionAuthenticationFilter : public IFilter {
 public:
  SessionAuthenticationFilter(SessionManager &sessions, std::string cookieName)
      : m_sessions(sessions), m_cookieName(std::move(cookieName)) {}

  bool doFilter(HttpRequest &request, HttpResponse &response) override {
    SecurityContextHolder::clearContext();

    std::string sessionId = request.cookie(m_cookieName);
    if (!sessionId.empty()) {
      auto auth = m_sessions.getSession(sessionId);
      if (auth) SecurityContextHolder::getContext().setAuthentication(*auth);
    }

    return true;
  }

 private:
  SessionManager &m_sessions;
  std::string m_cookieName;
};