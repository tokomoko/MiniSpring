#include"MiniSpringApp.h"

#include"../database/sqlite/SQLiteDatabase.h"
#include"../database/sqlite/SQLitePreparedStatement.h"
#include"../database/sqlite/SQLiteQueryResult.h"

MiniSpringApp::MiniSpringApp() : m_jsonMapper(m_entityRegistry), m_server(*this, m_jsonMapper) {
}

void MiniSpringApp::GET(const std::string& path, Router::Handler handler) {
	m_router.addRoute("GET", path, handler);
}

void MiniSpringApp::POST(const std::string& path, Router::Handler handler) {
	m_router.addRoute("POST", path, handler);
}

void MiniSpringApp::listen() {
	m_server.listen(m_serverConfiguration.port());
}

void MiniSpringApp::migrate() {
	if (!m_migrations) throw std::runtime_error("Migration manager not configured");
	m_migrations->migrate();
}

bool MiniSpringApp::handle(HttpRequest& request, HttpResponse& response) {
	if (m_router.handle(request, response)) return true;
	if (m_staticFiles && m_staticFiles->handle(request, response)) return true;
	return false;
}

void MiniSpringApp::build() {
	if (!m_databaseConfiguration.file().empty()) {
		auto sqlite = std::make_unique<SQLiteDatabase>();
		sqlite->connect((executableDirectory()/m_databaseConfiguration.file()).string());

		m_database = std::move(sqlite);
		m_entityManager = std::make_unique<EntityManager>(*m_database, m_entityRegistry);
	}

	if (!m_databaseConfiguration.migrations().empty()) {
		m_migrations = std::make_unique<MigrationManager>(*m_database, executableDirectory()/m_databaseConfiguration.migrations());
	}

	if (!m_staticFilesConfiguration.directory().empty()) {
		m_resources = std::make_unique<ResourceManager>(executableDirectory()/m_staticFilesConfiguration.directory());
		m_mimeTypes = std::make_unique<MimeTypeRegistry>();
		m_staticFiles = std::make_unique<StaticFileHandler>(*m_resources, *m_mimeTypes);
	}
}

void MiniSpringApp::initControllers() {
	m_controllers.initialize(m_router);
}

EntityManager& MiniSpringApp::entityManager() {
	return *m_entityManager;
}

EntityRegistry& MiniSpringApp::entities() {
	return m_entityRegistry;
}

MigrationManager& MiniSpringApp::migrationManager() {
	return *m_migrations;
}

JsonMapper& MiniSpringApp::jsonMapper() {
	return m_jsonMapper;
}

MarkdownRenderer& MiniSpringApp::MiniSpringApp::markdownRenderer() {
	return m_markDownRenderer;
}

ServerConfiguration& MiniSpringApp::server() {
	return m_serverConfiguration;
}


DatabaseConfiguration& MiniSpringApp::database() {
	return m_databaseConfiguration;
}


StaticFilesConfiguration& MiniSpringApp::staticFiles() {
	return m_staticFilesConfiguration;
}

ControllerRegistry& MiniSpringApp::controllers() {
	return m_controllers;
}