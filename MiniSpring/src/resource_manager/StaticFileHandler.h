#pragma once

#include"ResourceManager.h"

#include"../core/Core.h"

#include"../web/HttpRequest.h"
#include"../web/HttpResponse.h"
#include"../web/MimeTypeRegistry.h"

class MINISPRING_API StaticFileHandler {
public:
	StaticFileHandler(ResourceManager& resources, MimeTypeRegistry& mimeTypes);

	bool handle(HttpRequest& request, HttpResponse& response);

private:
	ResourceManager& m_resources;
	MimeTypeRegistry& m_mimeTypes;
};