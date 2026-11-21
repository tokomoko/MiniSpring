#pragma once

#include "Core.h"

class MINISPRING_API HttpRequest;
class MINISPRING_API HttpResponse;

class MINISPRING_API IRequestHandler {
 public:
  virtual ~IRequestHandler() = default;

  virtual bool handle(HttpRequest &request, HttpResponse &response) = 0;
};