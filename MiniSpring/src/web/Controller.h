#pragma once

#include "../core/Core.h"

#include "Router.h"

class MINISPRING_API Controller {
 public:
  virtual void registerRoutes(Router &router) = 0;
  virtual ~Controller() {}
};