#include"MiniSpringApp.h"

#include"../database/sqlite/SQLiteDatabase.h"
#include"../database/sqlite/SQLitePreparedStatement.h"
#include"../database/sqlite/SQLiteQueryResult.h"

MiniSpringApp::MiniSpringApp() : m_jsonMapper(m_entityRegistry), m_server(*this, m_jsonMapper) {
	m_database = std::make_unique<SQLiteDatabase>();
	m_entityManager = std::make_unique<EntityManager>(*m_database, m_entityRegistry);
}

void MiniSpringApp::GET(const std::string& path, Router::Handler handler) {
	m_router.addRoute("GET", path, handler);
}

void MiniSpringApp::POST(const std::string& path, Router::Handler handler) {
	m_router.addRoute("POST", path, handler);
}

void MiniSpringApp::listen(int port) {
	m_server.listen(port);
}

void MiniSpringApp::staticFiles(const std::filesystem::path& path) {
	m_resources = std::make_unique<ResourceManager>(executableDirectory() / path);
	m_mimeTypes = std::make_unique<MimeTypeRegistry>();
	m_staticFiles = std::make_unique<StaticFileHandler>(*m_resources, *m_mimeTypes);
}

void MiniSpringApp::database(const std::filesystem::path& path) {
	auto sqlite = std::make_unique<SQLiteDatabase>();
	sqlite->connect((executableDirectory()/path).string());
	m_database = std::move(sqlite);
	m_entityManager = std::make_unique<EntityManager>(*m_database, m_entityRegistry);
}

void MiniSpringApp::migrations(const std::filesystem::path& path) {
	if (!m_database) throw std::runtime_error("Database must be configured before migrations");
	m_migrations = std::make_unique<MigrationManager>(*m_database, executableDirectory() / path);
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