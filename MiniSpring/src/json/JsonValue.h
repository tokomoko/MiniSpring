#pragma once

#include "../core/Core.h"
#include "../core/Value.h"

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class MINISPRING_API JsonValue;

using JsonArray = std::vector<JsonValue>;

using JsonObject = std::unordered_map<std::string, JsonValue>;

class MINISPRING_API JsonValue {
  using Type = std::variant<Value, JsonArray, JsonObject>;

 public:
  JsonValue();
  JsonValue(const Value &value);
  JsonValue(const char *value);
  JsonValue(std::string value);
  JsonValue(double value);
  JsonValue(int value);
  JsonValue(bool value);
  JsonValue(JsonArray value);
  JsonValue(JsonObject value);

  std::string stringify() const;
  std::string asString() const;
  JsonValue &operator[](const std::string &key);

  template <typename T>
  const T &as() const {
    if (!is<T>()) throw std::runtime_error("Invalid Value conversion");
    return std::get<T>(m_value);
  }

  template <typename T>
  bool is() const {
    return std::holds_alternative<T>(m_value);
  }

  template <typename T>
  const T &get() const {
    return std::get<T>(m_value);
  }

 private:
  Type m_value;
};