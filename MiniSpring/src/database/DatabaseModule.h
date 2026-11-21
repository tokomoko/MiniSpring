#pragma once

#include "../module/Module.h"

#include "DatabaseConfiguration.h"
#include "DatabaseContext.h"

class MINISPRING_API DatabaseModule : public Module {
 public:
  DatabaseConfiguration &configuration() { return m_configuration; }

  void migrate() {
    if (m_ctx.m_migrationManager) m_ctx.m_migrationManager->migrate();
  }

  void build(ServiceProvider &services) override;

 private:
  DatabaseConfiguration m_configuration;
  DatabaseContext m_ctx;
};