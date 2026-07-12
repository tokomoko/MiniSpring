#pragma once

#include"IRequestHandler.h"

#include"Core.h"

#include"../web/Router.h"
#include"../web/HttpServer.h"
#include"../web/Controller.h"
#include"../web/ControllerRegistry.h"
#include"../web/MimeTypeRegistry.h"

#include"../resource_manager/Platform.h"
#include"../resource_manager/StaticFileHandler.h"
#include"../resource_manager/ResourceManager.h"

#include"../database/Database.h"
#include"../database/DatabaseException.h"
#include"../database/PreparedStatement.h"
#include"../database/QueryResult.h"

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

#include"../json/JsonValue.h"
#include"../json/JsonParser.h"
#include"../json/JsonSerializer.h"
#include"../json/JsonMapper.h"

#include"../markdown/MarkdownRenderer.h"

#include"../config/DatabaseConfiguration.h"
#include"../config/ServerConfiguration.h"
#include"../config/StaticFilesConfiguration.h"

class MINISPRING_API MiniSpringApp : public IRequestHandler {
public:
	MiniSpringApp();
	MiniSpringApp(const MiniSpringApp&) = delete;
	MiniSpringApp& operator=(const MiniSpringApp&) = delete;

	void GET(const std::string& path, Router::Handler handler);
	void POST(const std::string& path, Router::Handler handler);
	bool handle(HttpRequest& request, HttpResponse& response) override;
	void migrate();
	void listen();
	void build();
	void initControllers();
	
	EntityManager& entityManager();
	EntityRegistry& entities();
	MigrationManager& migrationManager();
	JsonMapper& jsonMapper();
	MarkdownRenderer& markdownRenderer();
	ServerConfiguration& server();
	DatabaseConfiguration& database();
	StaticFilesConfiguration& staticFiles();
	ControllerRegistry& controllers();

	template<typename T>
	void registerEntity() {
		m_entityRegistry.registerEntity<T>();
	}

private:
	EntityRegistry m_entityRegistry;
	JsonMapper m_jsonMapper;
	Router m_router;
	HttpServer m_server;
	MarkdownRenderer m_markDownRenderer;

	ServerConfiguration m_serverConfiguration;
	DatabaseConfiguration m_databaseConfiguration;
	StaticFilesConfiguration m_staticFilesConfiguration;

	ControllerRegistry m_controllers;

	std::unique_ptr<Database> m_database;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<MigrationManager> m_migrations;
	std::unique_ptr<ResourceManager> m_resources;
	std::unique_ptr<MimeTypeRegistry> m_mimeTypes;
	std::unique_ptr<StaticFileHandler> m_staticFiles;
};