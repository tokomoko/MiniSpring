#include"Router.h"

#include"../json/JsonValue.h"

#include<sstream>

void Router::addRoute(const std::string& method, const std::string& path, Handler handler, RouteMetadata metadata) {
	m_routes.push_back({ handler, method, path, std::move(metadata) });
}

const Router::Route* Router::findRoute(const std::string& method, const std::string& path, HttpRequest& request) {
	for (auto& route : m_routes) {
		if (route.method != method) continue;
		if (!matchPath(route.path, path, request)) continue;
		return &route;
	}
	return nullptr;
}

bool Router::handle(HttpRequest& request, HttpResponse& response) {
	const Route* route = findRoute(request.method(), request.path(), request);
	if (!route) return false;

	auto result = route->handler(request);
	if (result.isOk()) response = result.value();
	else {
		auto& error = result.error();
		response.status(error.status());
		response.json(error.toJson());
	}

	return true;
}

bool Router::matchPath(const std::string& pattern, const std::string& actual, HttpRequest& request) {
	std::vector<std::string> patternParts;
	std::vector<std::string> actualParts;
	std::stringstream p(pattern);
	std::stringstream a(actual);
	std::string part;

	while (std::getline(p, part, '/')) {
		if (!part.empty()) patternParts.push_back(part);
	}

	while (std::getline(a, part, '/')) {
		if (!part.empty()) actualParts.push_back(part); 
	}

	if (patternParts.size() != actualParts.size()) return false;

	std::unordered_map<std::string, std::string> params;

	for (size_t i = 0; i < patternParts.size(); i++) {
		auto& pPart = patternParts[i];
		auto& aPart = actualParts[i];

		if (pPart.front() == '{' && pPart.back() == '}') {
			std::string name = pPart.substr(1, pPart.size() - 2);
			params[name] = aPart;
		}
		else if (pPart != aPart) {
			return false;
		}
	}

	for (auto&[key, value] : params)
		request.setPathParameter( key, value);

	return true;
}