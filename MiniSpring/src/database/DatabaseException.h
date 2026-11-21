#pragma once

#include "../core/Core.h"

#include <exception>
#include <string>

class DatabaseException : public std::exception {
 public:
  explicit DatabaseException(std::string message, int errorCode = 0,
                             std::string sql = "")
      : m_rawMessage(message),
        m_errorCode(errorCode),
        m_sql(std::move(sql)),
        m_fullMessage(buildFullMessage(message, errorCode, m_sql)) {}

  const char *what() const noexcept override { return m_fullMessage.c_str(); }

  int errorCode() const noexcept { return m_errorCode; }

  const std::string &rawMessage() const noexcept { return m_rawMessage; }

  const std::string &sql() const noexcept { return m_sql; }

 private:
  static std::string buildFullMessage(const std::string &message, int errorCode,
                                      const std::string &sql) {
    std::string result = message;

    if (errorCode != 0) {
      result += " [code " + std::to_string(errorCode) + "]";
    }

    if (!sql.empty()) {
      result += " | SQL: " + sql;
    }

    return result;
  }

  std::string m_rawMessage;
  int m_errorCode;
  std::string m_sql;
  std::string m_fullMessage;
};