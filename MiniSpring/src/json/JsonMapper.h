#pragma once

#include "../core/Core.h"

#include "../orm/EntityRegistry.h"
#include "JsonParser.h"
#include "JsonSerializer.h"
#include "JsonValue.h"

#include <vector>

class MINISPRING_API JsonMapper {
 public:
  JsonMapper(EntityRegistry &registry) : m_registry(registry) {}

  template <typename T>
  T fromJson(const JsonValue &value) {
    auto &descriptor = m_registry.get<T>();
    return JsonSerializer::deserialize<T>(value, descriptor.type());
  }

  template <typename T>
  JsonValue toJson(const T &object) {
    auto &descriptor = m_registry.get<T>();
    return JsonSerializer::serialize<T>(object, descriptor.type());
  }

  template <typename T>
  JsonValue toJson(const std::vector<T> &objects) {
    auto &descriptor = m_registry.get<T>();
    return JsonSerializer::serialize<T>(objects, descriptor.type());
  }

 private:
  EntityRegistry &m_registry;
};