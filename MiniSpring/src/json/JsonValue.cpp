#include "JsonValue.h"

#include <sstream>

JsonValue::JsonValue() : m_value(Value(nullptr)) {}

JsonValue::JsonValue(const char* v) : m_value(Value(std::string(v))) {}

JsonValue::JsonValue(std::string v) : m_value(Value(v)) {}

JsonValue::JsonValue(double v) : m_value(Value(v)) {}

JsonValue::JsonValue(int v) : m_value(Value((double)v)) {}

JsonValue::JsonValue(bool v) : m_value(Value(v)) {}

JsonValue::JsonValue(JsonArray v) : m_value(v) {}

JsonValue::JsonValue(JsonObject v) : m_value(v) {}

std::string JsonValue::asString() const {
	return std::get<Value>(m_value).get<std::string>();
}

std::string JsonValue::stringify() const {

	if (std::holds_alternative<Value>(m_value))
		return std::get<Value>(m_value).toJsonString();

	if (is<JsonArray>()) {
		auto& arr = std::get<JsonArray>(m_value);

		std::string result = "[";
		for (size_t i = 0; i < arr.size(); i++) {
			if (i) result += ",";
			result += arr[i].stringify();
		}

		result += "]";

		return result;
	}

	if (is<JsonObject>()) {
		auto& obj = std::get<JsonObject>(m_value);

		std::string result = "{";
		bool first = true;
		for (auto&[key, val] : obj) {
			if (!first) result += ",";
			result += "\"" + key + "\":" + val.stringify();
			first = false;
		}
		result += "}";

		return result;
	}

	return "";
}
