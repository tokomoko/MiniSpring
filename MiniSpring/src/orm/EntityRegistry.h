#pragma once

#include"../core/Core.h"

#include"EntityDescriptorBase.h"
#include"EntityDescriptor.h"

#include<unordered_map>
#include<typeindex>

class MINISPRING_API EntityRegistry
{
public:
	EntityRegistry() = default;

	EntityRegistry(const EntityRegistry&) = delete;
	EntityRegistry& operator=(const EntityRegistry&) = delete;

	EntityRegistry(EntityRegistry&&) noexcept = default;
	EntityRegistry& operator=(EntityRegistry&&) noexcept = default;

	template<typename T>
	void registerEntity(EntityDescriptor<T> descriptor) {
		auto owned = std::make_unique<EntityDescriptor<T>>(std::move(descriptor));
		m_entities[typeid(T)] = std::move(owned);
	}

	template<typename T>
	void registerEntity() {
		registerEntity(T::descriptor());
	}

	template<typename T>
	EntityDescriptor<T>& get() {
		auto it = m_entities.find(typeid(T));
		if (it == m_entities.end()) throw std::runtime_error("Entity not registered");

		return *static_cast<EntityDescriptor<T>*>(it->second.get());
	}

private:
	std::unordered_map<std::type_index, std::unique_ptr<EntityDescriptorBase>> m_entities;
};