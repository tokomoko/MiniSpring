#pragma once

#include<memory>
#include<typeindex>
#include<unordered_map>
#include<stdexcept>

class ServiceProvider {
public:
	template<typename T>
	void add(std::unique_ptr<T> service) {
		m_services[typeid(T)] = std::move(service);
	}

	template<typename T>
	void addRef(T& service) {
		m_services[typeid(T)] = std::shared_ptr<T>(&service, [](T*) {});
	}

    template<typename T>
    T& get() {
        auto typeKey = std::type_index(typeid(T));
        auto it = m_services.find(typeKey);

        if (it == m_services.end()) {
            throw std::runtime_error("Service not found in ServiceProvider: " + std::string(typeid(T).name()));
        }

        return *static_cast<T*>(it->second.get());
    }

private:
	std::unordered_map<std::type_index, std::shared_ptr<void>> m_services;
};