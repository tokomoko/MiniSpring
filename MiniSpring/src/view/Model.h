#pragma once

#include "../json/JsonValue.h"

#include <unordered_map>

class Model {
public:
	void add(const std::string& key, JsonValue value) {
		m_values[key] = value;
	}

	JsonValue& operator[](const std::string& key) {
		return m_values[key];
	}

	const JsonValue& get(const std::string& key) const {
		return m_values.at(key);
	}

	bool contains(const std::string& key) const{
		return m_values.find(key) != m_values.end();
	}

	const auto& values() const {
		return m_values;
	}

private:
	std::unordered_map<std::string, JsonValue> m_values;
};