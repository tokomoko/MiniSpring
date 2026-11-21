#pragma once

#include "../core/Core.h"

#include <functional>
#include <string>
#include <vector>

#include "FilterChain.h"
#include "HttpError.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

#include "Result.h"

class MINISPRING_API Router {
 public:
  using Handler = std::function<Result<HttpResponse, HttpError>(HttpRequest &)>;

  struct Route {
    Handler handler;
    std::shared_ptr<FilterChain> filters;
    std::string method;
    std::string path;
  };

  void addRoute(const std::string &method, const std::string &path,
                Handler handler,
                std::shared_ptr<FilterChain> filters = nullptr);
  bool handle(HttpRequest &request, HttpResponse &response);

  const Route *findRoute(const std::string &method, const std::string &path,
                         HttpRequest &request);

 private:
  bool matchPath(const std::string &pattern, const std::string &actual,
                 HttpRequest &request);

 private:
  std::vector<Route> m_routes;
};