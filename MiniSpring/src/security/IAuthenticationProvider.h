#pragma once

#include <optional>
#include <string>

#include "Authentication.h"

class IAuthenticationProvider {
 public:
  virtual ~IAuthenticationProvider() = default;
  virtual std::optional<Authentication> authenticate(
      const std::string &principal, const std::string &credentials) = 0;
};