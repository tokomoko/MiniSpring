#pragma once

#include "../models/Todo.h"

#include <MiniSpring.h>


class TodoController : public Controller {
public:
	TodoController(MiniSpringApp& app) : m_manager(app.entityManager()), m_mapper(app.jsonMapper()) {}

	void registerRoutes(Router& router) override {
		router.addRoute("GET", "/todos", [this](HttpRequest& req) {
			HttpResponse res;
			auto todos = m_manager.findAll<Todo>();
			res.json(m_mapper.toJson(todos));
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("GET", "/todos/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			int id = std::stoi(req.pathParameter("id"));
			auto todo = m_manager.find<Todo>(id);
			if (!todo) return Result<HttpResponse, HttpError>::error(HttpError(404, "Todo not found"));
			res.json(m_mapper.toJson(todo.value()));
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("POST", "/todos", [this](HttpRequest& req) {
			HttpResponse res;
			Todo todo = req.json<Todo>();
			m_manager.persist(todo);
			res.json(m_mapper.toJson(todo));
			return Result<HttpResponse, HttpError>::success(res);
		});

		router.addRoute("DELETE", "/todos/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			int id = std::stoi(req.pathParameter("id"));
			m_manager.removeById<Todo>(Value(id));
			res.status(204);
			return Result<HttpResponse, HttpError>::success(res);
		});

		/*router.addRoute("PUT", "/todos/{id}", [this](HttpRequest& req) {
			HttpResponse res;
			int id = std::stoi(req.pathParameter("id"));

			auto todo = m_manager.find<Todo>(id);
			if (!todo) return Result<HttpResponse, HttpError>::error(HttpError(404, "Todo not found"));

			//JsonValue json = req.json<JsonValue>();
			//bool completed = json["completed"].as<bool>();
			todo->completed = completed;
			m_manager.update(todo.value());
			res.json(m_mapper.toJson(todo.value()));
			return Result<HttpResponse, HttpError>::success(res);
		});*/
	}

private:
	EntityManager& m_manager;
	JsonMapper& m_mapper;
};