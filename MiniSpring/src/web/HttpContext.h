#pragma once

#include"Router.h"
#include"ControllerRegistry.h"
#include"MimeTypeRegistry.h"
#include"../json/JsonMapper.h"
#include"HttpServer.h"
#include"FilterChain.h"

#include<memory>

class HttpModule;

class HttpContext {
	friend class HttpModule;
public:
	Router& router() { return m_router; }
	ControllerRegistry& controllers() { return m_controllers; }
	MimeTypeRegistry& mimeTypes() { return m_mimeTypes; }
	JsonMapper& jsonMapper() { return *m_mapper; }
	FilterChain& filters() { return m_filterChain; }

private:
	Router m_router;
	ControllerRegistry m_controllers;
	MimeTypeRegistry m_mimeTypes;
	FilterChain m_filterChain;
	std::unique_ptr<JsonMapper> m_mapper;
	std::unique_ptr<HttpServer> m_server;
};