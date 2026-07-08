#include"UserController.h"

#include"../json/Json.h"

void UserController::registerRoutes(Router& router) {
	/*router.addRoute(
		"GET",
		"/user",
		[this](HttpRequest& req, HttpResponse& res) {
			getUser(req, res);
		});*/
}

void UserController::getUser(HttpRequest& req, HttpResponse& res)
{
	Json user;
	user["id"] = 10;
	user["name"] = "John";

	res.json(user);
}