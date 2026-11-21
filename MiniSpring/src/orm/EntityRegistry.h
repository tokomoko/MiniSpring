#pragma once

#include "../core/Core.h"

#include "EntityDescriptor.h"
#include "EntityDescriptorBase.h"

#include <string>
#include <typeindex>
#include <unordered_map>

class MINISPRING_API EntityRegistry {
 public:
  EntityRegistry() = default;

  EntityRegistry(const EntityRegistry &) = delete;
  EntityRegistry &operator=(const EntityRegistry &) = delete;

  EntityRegistry(EntityRegistry &&) noexcept = default;
  EntityRegistry &operator=(EntityRegistry &&) noexcept = default;

  template <typename T>
  EntityRegistry &registerEntity(EntityDescriptor<T> descriptor) {
    auto owned = std::make_unique<EntityDescriptor<T>>(std::move(descriptor));
    m_entities[typeid(T)] = std::move(owned);
    return *this;
  }

  template <typename T>
  EntityRegistry &registerEntity() {
    return registerEntity(T::descriptor());
  }

  template <typename T>
  EntityDescriptor<T> &get() {
    auto typeKey = std::type_index(typeid(T));
    auto it = m_entities.find(typeKey);

    if (it == m_entities.end()) {
      throw std::runtime_error(
          std::string("Entity not registered in EntityRegistry: ") +
          typeid(T).name());
    }

    return *static_cast<EntityDescriptor<T> *>(it->second.get());
  }

 private:
  std::unordered_map<std::type_index, std::unique_ptr<EntityDescriptorBase>>
      m_entities;
};