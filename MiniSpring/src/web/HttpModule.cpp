#include"HttpModule.h"

#include"../core/ServiceProvider.h"
#include"../orm/EntityRegistry.h"

#include<fstream>
#include<sstream>

void HttpModule::build(ServiceProvider& services) {
	m_ctx.m_mapper = std::make_unique<JsonMapper>(services.get<EntityRegistry>());

	m_ctx.m_server = std::make_unique<HttpServer>(*this, *m_ctx.m_mapper, m_configuration);

	services.addRef<Router>(m_ctx.router());
	services.addRef<ControllerRegistry>(m_ctx.controllers());
	services.addRef<MimeTypeRegistry>(m_ctx.mimeTypes());
	services.addRef<JsonMapper>(*m_ctx.m_mapper);
}
void HttpModule::start(ServiceProvider& services) {
	m_ctx.controllers().initialize(m_ctx.router());
	m_ctx.m_server->listen(m_configuration.port());
}

void HttpModule::stop(ServiceProvider& services) {
	// HttpServer::listen() is currently an unconditional `while (true)` with
	// no shutdown flag or socket-close hook it can react to, so there is no
	// clean way to stop it yet. Flagging rather than silently no-op'ing this —
	// worth a follow-up if you need graceful shutdown later.
}

bool HttpModule::handle(HttpRequest& request, HttpResponse& response) {
	if (m_ctx.router().handle(request, response))
		return true;

	return serveStaticFile(request, response);
}

bool HttpModule::serveStaticFile(HttpRequest& request, HttpResponse& response) {
	if (m_staticFiles.directory().empty())
		return false;

	std::string requestPath = request.path();
	if (!requestPath.empty() && requestPath.front() == '/')
		requestPath.erase(requestPath.begin());

	if (requestPath.empty())
		requestPath = "index.html";

	auto root = std::filesystem::weakly_canonical(m_staticFiles.directory());
	auto full = std::filesystem::weakly_canonical(root / requestPath);

	auto[rootEnd, fullBegin] = std::mismatch(root.begin(), root.end(), full.begin());
	if (rootEnd != root.end())
		return false;

	std::ifstream file(full, std::ios::binary);
	if (!file) return false;

	std::ostringstream contents;
	contents << file.rdbuf();

	response.header("Content-Type", m_ctx.mimeTypes().get(full.extension().string()));
	response.body(contents.str());
	return true;
}