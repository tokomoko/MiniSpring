#pragma once

#include "IAuthenticationProvider.h"
#include "IPasswordEncoder.h"
#include "IUserDetailsService.h"

class DaoAuthenticationProvider : public IAuthenticationProvider {
 public:
  DaoAuthenticationProvider(IUserDetailsService &userDetailsService,
                            IPasswordEncoder &passwordEncoder)
      : m_userDetailsService(userDetailsService),
        m_passwordEncoder(passwordEncoder) {}

  std::optional<Authentication> authenticate(
      const std::string &username, const std::string &rawPassword) override {
    auto user = m_userDetailsService.loadUserByUsername(username);
    if (!user) return std::nullopt;
    if (!(*user)->isEnabled()) return std::nullopt;
    if (!m_passwordEncoder.matches(rawPassword, (*user)->passwordHash()))
      return std::nullopt;

    return Authentication((*user)->username(), (*user)->authorities());
  }

 private:
  IUserDetailsService &m_userDetailsService;
  IPasswordEncoder &m_passwordEncoder;
};