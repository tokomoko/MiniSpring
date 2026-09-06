#pragma once

#include"../models/LoginRequest.h"

#include<MiniSpring.h>

#include<algorithm>

class AuthController : public Controller {
public:
	AuthController(MiniSpringApp& app)
		: m_authManager(app.services().get<AuthenticationManager>()),
		m_sessions(app.services().get<SessionManager>()),
		m_config(app.services().get<SecurityConfiguration>()),
		m_security(app.modules().get<SecurityModule>()) {}

	void registerRoutes(Router& router) override {
		router.addRoute("POST", "/api/login", [this](HttpRequest& req) {
			HttpResponse res;
			LoginRequest login = req.json<LoginRequest>();

			auto auth = m_authManager.authenticate(login.username, login.password);
			if (!auth) return Result<HttpResponse, HttpError>::error(HttpError(401, "Invalid username or password"));

			std::string sessionId = m_sessions.createSession(*auth);
			res.setCookie(m_config.cookieName(), sessionId);

			JsonObject body;
			body["username"] = auth->principal();
			res.json(JsonValue(body));

			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("GET", "/login", [this](HttpRequest& req) {
			HttpResponse res;
			std::string html = readFile("resources/static/login.html");
			res.html(html);
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("POST", "/api/logout", [this](HttpRequest& req) {
			HttpResponse res;
			std::string sessionId = req.cookie(m_config.cookieName());
			if (!sessionId.empty()) m_sessions.destroySession(sessionId);

			res.clearCookie(m_config.cookieName());
			res.status(204);
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("GET", "/api/me", [this](HttpRequest& req) {
			HttpResponse res;

			auto& auth = SecurityContextHolder::getContext().authentication();

			bool isAdmin = std::find(auth.authorities().begin(), auth.authorities().end(), "ADMIN") != auth.authorities().end();

			JsonObject body;
			body["username"] = auth.principal();
			body["admin"] = isAdmin;

			res.json(JsonValue(body));

			return Result<HttpResponse, HttpError>::success(res);

		}, m_security.protect());
	}

private:
	static std::string readFile(const std::string& path) {
		std::ifstream file(path, std::ios::binary);
		if (!file) {
			throw std::runtime_error("AuthController: could not open file: " + path);
		}
		std::ostringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}

private:
	AuthenticationManager& m_authManager;
	SessionManager& m_sessions;
	SecurityConfiguration& m_config;
	SecurityModule& m_security;
};