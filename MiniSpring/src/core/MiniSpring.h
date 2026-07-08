#pragma once

#include"IRequestHandler.h"

#include"../web/Router.h"
#include"../web/HttpServer.h"
#include"../web/Controller.h"
#include"../web/MimeTypeRegistry.h"

#include"../resource_manager/Platform.h"
#include"../resource_manager/StaticFileHandler.h"
#include"../resource_manager/ResourceManager.h"

class MiniSpring : public IRequestHandler {
public:
	MiniSpring();

	void GET(const std::string& path, Router::Handler handler);
	void POST(const std::string& path, Router::Handler handler);

	void listen(int port);

	template<typename T>
	void registerController() {
		controllers.push_back(std::make_unique<T>());
		controllers.back()->registerRoutes(m_router);
	}

	void staticFiles(const std::filesystem::path& path);

	bool handle(HttpRequest& request, HttpResponse& response) override;

private:
	Router m_router;
	HttpServer m_server;
	std::vector<std::unique_ptr<Controller>>controllers;

	std::unique_ptr<ResourceManager> m_resources;
	std::unique_ptr<MimeTypeRegistry> m_mimeTypes;
	std::unique_ptr<StaticFileHandler> m_staticFiles;
};