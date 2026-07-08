#pragma once

#include"../core/Value.h"

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>

class JsonValue;

using JsonArray = std::vector<JsonValue>;

using JsonObject = std::unordered_map<std::string, JsonValue>;

class JsonValue {
	using Type = std::variant<Value, JsonArray, JsonObject>;
public:

	JsonValue();
	JsonValue(const char* value);
	JsonValue(std::string value);
	JsonValue(double value);
	JsonValue(int value);
	JsonValue(bool value);
	JsonValue(JsonArray value);
	JsonValue(JsonObject value);

	std::string stringify() const;
	std::string asString() const;

	template<typename T>
	T as() const {
		return get<Value>().get<T>();
	}

	template<typename T>
	bool is() const {
		return std::holds_alternative<T>(m_value);
	}

	template<typename T>
	const T& get() const {
		return std::get<T>(m_value);
	}

private:
	Type m_value;
};