#pragma once

#include "../core/Core.h"

#include <string>
#include <vector>

class MINISPRING_API Authentication {
 public:
  Authentication() = default;
  Authentication(std::string principal, std::vector<std::string> authorities)
      : m_principal(std::move(principal)),
        m_authorities(std::move(authorities)),
        m_authenticated(true) {}

  const std::string &principal() const { return m_principal; }
  const std::vector<std::string> &authorities() const { return m_authorities; }
  bool isAuthenticated() const { return m_authenticated; }

 private:
  std::string m_principal;
  std::vector<std::string> m_authorities;
  bool m_authenticated = false;
};