#pragma once

#include"../models/Post.h"
#include"../models/PostCreateRequest.h"
#include"../models/PostUpdateRequest.h"

#include<MiniSpring.h>

#include<algorithm>
#include<cctype>
#include<ctime>
#include<fstream>
#include<sstream>

class PostController : public Controller {
public:
	PostController(MiniSpringApp& app) : m_manager(app.entityManager()), m_mapper(app.jsonMapper()), m_markdownRenderer(app.markdownRenderer()) {}

	void registerRoutes(Router& router) override {
		router.addRoute("GET", "/posts/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			std::string html = readFile((executableDirectory()/"resources/static/post.html").string());
			res.html(html);
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("GET", "/api/posts", [this](HttpRequest& req) {
			HttpResponse res;
			auto posts = m_manager.findAll<Post>();

			std::sort(posts.begin(), posts.end(), [](const Post& a, const Post& b) {
				return a.createdAt > b.createdAt; // newest first
			});

			res.json(m_mapper.toJson(posts));
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("GET", "/api/posts/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			std::string id = req.pathParameter("id");
			auto post = m_manager.find<Post>(id);
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
			Post post;
			post.id = uniqueSlug(slugify(request.title));
			post.title = request.title;
			post.content = request.content;
			post.createdAt = static_cast<int>(std::time(nullptr));

			m_manager.persist(post);

			res.json(m_mapper.toJson(post));
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("PUT", "/api/posts/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			std::string id = req.pathParameter("id");
			auto post = m_manager.find<Post>(id);
			if (!post) return Result<HttpResponse, HttpError>::error(HttpError(404, "Post not found"));
			PostUpdateRequest update = req.json<PostUpdateRequest>();
			if (!update.title.empty()) post->title = update.title;
			post->content = update.content;
			m_manager.update(post.value());

			res.json(m_mapper.toJson(post.value()));
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("DELETE", "/api/posts/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			std::string id = req.pathParameter("id");
			m_manager.removeById<Post>(Value(id));
			res.status(204);
			return Result<HttpResponse, HttpError>::success(res);
		});
	}

private:
	static std::string readFile(const std::string& path) {
		std::ifstream file(path, std::ios::binary);
		if (!file) {
			throw std::runtime_error("PostController: could not open file: " + path);
		}
		std::ostringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}

	std::string slugify(const std::string& title) {
		std::string slug;
		slug.reserve(title.size());

		for (unsigned char c : title) {
			if (c >= 0x80) {
				slug += static_cast<char>(c);
			}
			else if (std::isalnum(c)) {
				slug += static_cast<char>(std::tolower(c));
			}
			else if (!slug.empty() && slug.back() != '-') {
				slug += '-';
			}
		}

		while (!slug.empty() && slug.back() == '-') {
			slug.pop_back();
		}

		return slug.empty() ? "post" : slug;
	}

	std::string uniqueSlug(const std::string& base) {
		std::string slug = base;
		int suffix = 2;

		while (m_manager.find<Post>(slug)) {
			slug = base + "-" + std::to_string(suffix);
			suffix++;
		}

		return slug;
	}

	EntityManager& m_manager;
	JsonMapper& m_mapper;
	MarkdownRenderer& m_markdownRenderer;
};