#pragma once

#include"../core/Core.h"

#include<string>
#include<vector>
#include<functional>

#include"HttpRequest.h"
#include"HttpResponse.h"
#include"HttpError.h"
#include"RouteMetadata.h"

#include"Result.h"

class MINISPRING_API Router {
public:
	using Handler = std::function<Result<HttpResponse, HttpError>(HttpRequest&)>;

	struct Route {
		Handler handler;
		std::string method;
		std::string path;
		RouteMetadata metadata;
	};

	void addRoute(const std::string& method, const std::string& path, Handler handler, RouteMetadata metadata = {});
	bool handle(HttpRequest& request, HttpResponse& response);

	const Route* findRoute(const std::string& method, const std::string& path, HttpRequest& request);

private:
	bool matchPath(const std::string& pattern, const std::string& actual, HttpRequest& request);

private:
	std::vector<Route> m_routes;
};