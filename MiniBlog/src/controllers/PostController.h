#pragma once

#include"../models/Post.h"
#include"../models/PostCreateRequest.h"
#include"../models/PostUpdateRequest.h"

#include"../services/PostService.h"

#include<MiniSpring.h>

#include<algorithm>
#include<fstream>
#include<sstream>

class PostController : public Controller {
public:
	PostController(MiniSpringApp& app)
		: m_service(app.services().get<PostService>()),
		m_mapper(app.services().get<JsonMapper>()),
		m_markdownRenderer(app.services().get<MarkdownRenderer>()),
		m_security(app.modules().get<SecurityModule>()) {}

	void registerRoutes(Router& router) override {
		router.addRoute("GET", "/posts/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			std::string html = readFile((executableDirectory() / "resources/static/post.html").string());
			res.html(html);
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("GET", "/api/posts", [this](HttpRequest& req) {
			HttpResponse res;
			auto posts = m_service.getAllPosts();
			res.json(m_mapper.toJson(posts));
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("GET", "/api/posts/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			std::string id = req.pathParameter("id");
			auto post = m_service.getPost(id);
			if (!post) return Result<HttpResponse, HttpError>::error(HttpError(404, "Post not found"));
			auto json = m_mapper.toJson(post.value());
			json["html"] = m_markdownRenderer.render(post->content);
			res.json(json);
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("POST", "/api/posts", [this](HttpRequest& req) {
			HttpResponse res;
			PostCreateRequest request = req.json<PostCreateRequest>();
			if (request.title.empty()) return Result<HttpResponse, HttpError>::error(HttpError(400, "Title is required"));
			Post post = m_service.create(request);
			res.json(m_mapper.toJson(post));
			return Result<HttpResponse, HttpError>::success(res);
		}, m_security.protect({ "ADMIN" }));

		router.addRoute("PUT", "/api/posts/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			std::string id = req.pathParameter("id");
			PostUpdateRequest update = req.json<PostUpdateRequest>();
			auto post = m_service.update(id, update);
			res.json(m_mapper.toJson(post));
			return Result<HttpResponse, HttpError>::success(res);
		}, m_security.protect({ "ADMIN" }));

		router.addRoute("DELETE", "/api/posts/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			std::string id = req.pathParameter("id");
			m_service.deletePost(id);
			res.status(204);
			return Result<HttpResponse, HttpError>::success(res);
		}, m_security.protect({ "ADMIN" }));
	}

private:
	static std::string readFile(const std::string& path) {
		std::ifstream file(path, std::ios::binary);
		if (!file) throw std::runtime_error("PostController: could not open file: " + path);
		std::ostringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}

	PostService& m_service;
	JsonMapper& m_mapper;
	MarkdownRenderer& m_markdownRenderer;
	SecurityModule& m_security;
};