#include "DatabaseModule.h"

#include "sqlite/SQLiteDatabase.h"

#include "../core/ServiceProvider.h"
#include "../orm/EntityRegistry.h"

void DatabaseModule::build(ServiceProvider& services) {
	auto sqlite = std::make_unique<SQLiteDatabase>();
	sqlite->connect(m_configuration.file().string());
	m_ctx.m_database = std::move(sqlite);

	auto& entities = services.get<EntityRegistry>();
	m_ctx.m_entityManager = std::make_unique<EntityManager>(*m_ctx.m_database, entities);

	if (!m_configuration.migrations().empty()) {
		m_ctx.m_migrationManager = std::make_unique<MigrationManager>(*m_ctx.m_database, m_configuration.migrations());
	}

	services.addRef<Database>(m_ctx.database());
	services.addRef<EntityManager>(m_ctx.entityManager());
	if (m_ctx.m_migrationManager) {
		services.addRef<MigrationManager>(m_ctx.migrationManager());
	}
}