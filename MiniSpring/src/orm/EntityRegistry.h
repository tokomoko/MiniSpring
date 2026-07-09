#pragma once

#include"EntityDescriptorBase.h"
#include"EntityDescriptor.h"

#include<unordered_map>
#include<typeindex>

class EntityRegistry
{
public:
	template<typename T>
	void registerEntity(EntityDescriptor<T>& descriptor) {
		m_entities[typeid(T)] = &descriptor;
	}


	template<typename T>
	EntityDescriptor<T>& get() {
		return *static_cast<EntityDescriptor<T>*>(m_entities[typeid(T)]);
	}

private:
	std::unordered_map<std::type_index, EntityDescriptorBase*> m_entities;
};