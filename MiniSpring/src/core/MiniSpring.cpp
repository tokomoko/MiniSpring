#include"MiniSpring.h"

MiniSpring::MiniSpring() : m_server(*this) {}

void MiniSpring::GET(const std::string& path, Router::Handler handler) {
	m_router.addRoute("GET", path, handler);
}

void MiniSpring::POST(const std::string& path, Router::Handler handler) {
	m_router.addRoute("POST", path, handler);
}

void MiniSpring::listen(int port) {
	//MigrationManager migrations(m_database,"resources/migrations");
	//migrations.migrate();

	m_server.listen(port);
}

void MiniSpring::staticFiles(const std::filesystem::path& path) {
	m_resources = std::make_unique<ResourceManager>(executableDirectory() / path);
	m_mimeTypes = std::make_unique<MimeTypeRegistry>();
	m_staticFiles = std::make_unique<StaticFileHandler>(*m_resources, *m_mimeTypes);
}

bool MiniSpring::handle(HttpRequest& request, HttpResponse& response) {
	if (m_router.handle(request, response)) return true;
	if (m_staticFiles && m_staticFiles->handle(request, response)) return true;
	return false;
}