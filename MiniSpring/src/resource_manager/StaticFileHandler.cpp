#include"StaticFileHandler.h"

#include"ResourceException.h"

#include"../web/MimeTypeRegistry.h"
#include"../web/exceptions/HttpException.h"
#include"../web/exceptions/NotFoundException.h"

#include"../utils/FileUtils.h"

#include<iostream>
#include<string>

StaticFileHandler::StaticFileHandler(ResourceManager& resources, MimeTypeRegistry& mimeTypes) : m_resources(resources), m_mimeTypes(mimeTypes) {}

bool StaticFileHandler::handle(HttpRequest& request, HttpResponse& response) {
	std::cout << "Static handler called\n";

	std::string path = request.path();
	std::cout << "Requested path: " << path << "\n";

	if (path == "/") path = "/index.html";
	if (path.compare(0, 1, "/") == 0) path.erase(0, 1);
	if (!m_resources.exists(path)) return false;

	try {
		auto content = m_resources.read(path);
		response.body(content);
		response.header("Content-Type", m_mimeTypes.get(extension(path)));
	}
	catch (ResourceException& e) {
		switch (e.type()) {
		case ResourceException::Type::NotFound: throw NotFoundException(e.what());
		case ResourceException::Type::InvalidPath: throw HttpException(400, e.what());
		case ResourceException::Type::ReadError: throw HttpException(500, e.what());
		}

		return false;
	}

	return true;
}