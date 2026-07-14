#pragma once

#include"../core/Core.h"
#include"../module/Module.h"
#include"../core/IRequestHandler.h"

#include"ServerConfiguration.h"
#include"StaticFilesConfiguration.h"
#include"HttpContext.h"

#include<filesystem>

class ServiceProvider;

class MINISPRING_API HttpModule : public Module, public IRequestHandler {
public:
	ServerConfiguration& configuration() { return m_configuration; }
	StaticFilesConfiguration& staticFiles() { return m_staticFiles; }
	ControllerRegistry& controllers() { return m_ctx.controllers(); }
	Router& router() { return m_ctx.router(); }
	MimeTypeRegistry& mimeTypes() { return m_ctx.mimeTypes(); }

	void build(ServiceProvider& services) override;
	void start(ServiceProvider& services) override;
	void stop(ServiceProvider& services) override;

	bool handle(HttpRequest& request, HttpResponse& response) override;

private:
	bool serveStaticFile(HttpRequest& request, HttpResponse& response);

private:
	ServerConfiguration m_configuration;
	StaticFilesConfiguration m_staticFiles;
	HttpContext m_ctx;
};