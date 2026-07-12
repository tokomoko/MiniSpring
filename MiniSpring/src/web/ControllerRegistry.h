#pragma once

#include <vector>
#include <memory>

#include "Controller.h"

class ControllerRegistry {
public:
	template<typename T, typename... Args>
	void registerController(Args&&... args) {
		auto controller = std::make_unique<T>(std::forward<Args>(args)...);
		m_controllers.push_back(std::move(controller));
	}

	void initialize(Router& router) {
		for (auto& controller : m_controllers) {
			controller->registerRoutes(router);
		}
	}

private:
	std::vector<std::unique_ptr<Controller>> m_controllers;
};