#pragma once

#include "../orm/EntityManager.h"
#include "Database.h"
#include "migration_manager/MigrationManager.h"

#include <memory>

class DatabaseModule;

class DatabaseContext {
  friend class DatabaseModule;

 public:
  Database &database() { return *m_database; }

  EntityManager &entityManager() { return *m_entityManager; }

  MigrationManager &migrationManager() { return *m_migrationManager; }

 private:
  std::unique_ptr<Database> m_database;
  std::unique_ptr<EntityManager> m_entityManager;
  std::unique_ptr<MigrationManager> m_migrationManager;
};