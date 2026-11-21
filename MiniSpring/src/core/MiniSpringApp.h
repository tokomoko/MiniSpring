#pragma once

#include "Core.h"
#include "IRequestHandler.h"

#include "../module/ModuleRegistry.h"
#include "ServiceProvider.h"

class MINISPRING_API MiniSpringApp : public IRequestHandler {
 public:
  MiniSpringApp();
  MiniSpringApp(const MiniSpringApp &) = delete;
  MiniSpringApp &operator=(const MiniSpringApp &) = delete;

  virtual bool handle(HttpRequest &request, HttpResponse &response) {
    return false;
  }

  void configure();
  void build();
  void listen();
  void stop();

  ModuleRegistry &modules() { return m_modules; }
  ServiceProvider &services() { return m_services; }

 private:
  ModuleRegistry m_modules;
  ServiceProvider m_services;
};