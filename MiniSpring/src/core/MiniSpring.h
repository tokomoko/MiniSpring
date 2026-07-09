#pragma once

#include"IRequestHandler.h"

#include"../web/Router.h"
#include"../web/HttpServer.h"
#include"../web/Controller.h"
#include"../web/MimeTypeRegistry.h"

#include"../resource_manager/Platform.h"
#include"../resource_manager/StaticFileHandler.h"
#include"../resource_manager/ResourceManager.h"

#include"../database/Database.h"
#include"../database/DatabaseException.h"
#include"../database/PreparedStatement.h"
#include"../database/QueryResult.h"

#include"../database/sqlite/SQLiteDatabase.h"
#include"../database/sqlite/SQLitePreparedStatement.h"
#include"../database/sqlite/SQLiteQueryResult.h"
#include"../database/sqlite/SQLiteUtils.h"

#include"../database/migration_manager/Migration.h"
#include"../database/migration_manager/MigrationManager.h"
#include"../database/migration_manager/MigrationUtils.h"

#include"../reflection/FieldBase.h"
#include"../reflection/FieldInfo.h"
#include"../reflection/FieldMetadata.h"
#include"../reflection/TypeDescriptor.h"

#include"../orm/EntityDescriptor.h"
#include"../orm/EntityRegistry.h"
#include"../orm/EntityManager.h"

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
	EntityRegistry m_entityRegistry;
	EntityManager m_entityManager;

	std::unique_ptr<Database> m_database;
	std::unique_ptr<ResourceManager> m_resources;
	std::unique_ptr<MimeTypeRegistry> m_mimeTypes;
	std::unique_ptr<StaticFileHandler> m_staticFiles;

	std::vector<std::unique_ptr<Controller>>controllers;
};