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
	PreFilterChain& preFilters() { return m_preFilterChain; }
	PostFilterChain& postFilters() { return m_postFilterChain; }

private:
	Router m_router;
	ControllerRegistry m_controllers;
	MimeTypeRegistry m_mimeTypes;
	PreFilterChain m_preFilterChain;
	PostFilterChain m_postFilterChain;
	std::unique_ptr<JsonMapper> m_mapper;
	std::unique_ptr<HttpServer> m_server;
};