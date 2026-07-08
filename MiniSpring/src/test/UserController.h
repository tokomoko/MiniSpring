#pragma once

#include"../web/Controller.h"
#include"../web/Router.h"

class UserController : public Controller {
public:
	void registerRoutes(Router& router) override;
	   	 
private:
	void getUser(HttpRequest& req, HttpResponse& res);
};