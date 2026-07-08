#pragma once

#include"Router.h"

class Controller {
public:
	virtual void registerRoutes(Router& router) = 0;
	virtual ~Controller() {}
};