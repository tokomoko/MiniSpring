#pragma once

#include "EntityManager.h"

#include"../core/Core.h"

#include <optional>
#include <vector>

template<typename T>
class Repository {
public:
	Repository(EntityManager& manager) : m_manager(manager) {}

	void save(const T& entity) {
		m_manager.save(entity);
	}

	std::optional<T> findById(Value id) {
		return m_manager.find<T>(id);
	}

	std::vector<T> findAll() {
		return m_manager.findAll<T>();
	}

	void update(const T& entity) {
		m_manager.update(entity);
	}

	void remove(const T& entity) {
		m_manager.remove(entity);
	}

private:
	EntityManager& m_manager;
};