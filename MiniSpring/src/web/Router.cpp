#include"Router.h"

#include"../json/JsonValue.h"

void Router::addRoute(const std::string& method,const std::string& path, Handler handler)
{
	m_routes.push_back({ handler, method, path });
}

bool Router::handle(HttpRequest& request, HttpResponse& response) {
	for (auto& route : m_routes) {
		if (route.method == request.method() && route.path == request.path()) {
			auto result = route.handler(request);
			if (result.isOk()) response = result.value();
			else {
				auto& error = result.error();
				response.status(error.status());
				response.json(error.toJson());
			}

			return true;
		}
	}

	return false;
}
