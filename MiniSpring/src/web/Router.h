#pragma once

#include<string>
#include<vector>
#include<functional>

#include"HttpRequest.h"
#include"HttpResponse.h"
#include"HttpError.h"

#include"Result.h"

class Router {
public:
	using Handler = std::function<Result<HttpResponse, HttpError>(HttpRequest&)>;

	void addRoute(const std::string& method, const std::string& path, Handler handler);
	bool handle(HttpRequest& request, HttpResponse& response);

private:
	struct Route {
		Handler handler;
		std::string method;
		std::string path;
	};

	std::vector<Route> m_routes;
};