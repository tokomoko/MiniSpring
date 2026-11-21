#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Module.h"

class ServiceProvider;

class ModuleRegistry {
 public:
  template <typename T, typename... Args>
  T &add(Args &&... args) {
    auto module = std::make_unique<T>(std::forward<Args>(args)...);
    T &ref = *module;
    m_lookup[typeid(T)] = &ref;
    m_modules.push_back(std::move(module));
    return ref;
  }

  template <typename T>
  T &get() {
    return static_cast<T &>(*m_lookup.at(typeid(T)));
  }

  void configure(ServiceProvider &services);
  void build(ServiceProvider &services);
  void start(ServiceProvider &services);
  void stop(ServiceProvider &services);

 private:
  std::vector<std::unique_ptr<Module>> m_modules;
  std::unordered_map<std::type_index, Module *> m_lookup;
};