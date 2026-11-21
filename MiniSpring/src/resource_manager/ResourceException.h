#pragma once

#include "../core/Core.h"

#include <exception>
#include <string>

class ResourceException : public std::exception {
 public:
  enum class Type { NotFound, InvalidPath, ReadError };

  ResourceException(Type type, std::string message)
      : m_type(type), m_message(std::move(message)) {}

  Type type() const { return m_type; }

  const char *what() const noexcept override { return m_message.c_str(); }

 private:
  Type m_type;
  std::string m_message;
};