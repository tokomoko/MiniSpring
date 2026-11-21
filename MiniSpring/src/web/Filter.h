#pragma once

#include "../core/Core.h"

class HttpRequest;
class HttpResponse;

class MINISPRING_API IFilter {
 public:
  virtual ~IFilter() = default;
  virtual bool doFilter(HttpRequest &request, HttpResponse &response) = 0;
};