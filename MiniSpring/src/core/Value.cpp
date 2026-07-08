#include "Value.h"

Value::Value() : m_value(nullptr) {}
Value::Value(std::nullptr_t value) : m_value(value) {}
Value::Value(bool value) : m_value(value) {}
Value::Value(int value) : m_value(value) {}
Value::Value(double value) : m_value(value) {}
Value::Value(const char* value) : m_value(std::string(value)) {}
Value::Value(std::string value) : m_value(std::move(value)) {}

std::string Value::toString() const {
	if (isNull()) return "null";
	if (isBoolean()) return get<bool>() ? "true" : "false";
	if (isInteger()) return std::to_string(get<int>());
	if (isDouble()) return std::to_string(get<double>());

	return get<std::string>();
}

std::string Value::typeName() const {
	if (isNull()) return "null";
	if (isBoolean()) return "bool";
	if (isInteger()) return "int";
	if (isDouble()) return "double";
	if (isString()) return "string";

	return "unknown";
}

std::string Value::toJsonString() const {
	if (isNull()) return "null";
	if (isBoolean()) return get<bool>() ? "true" : "false";
	if (isInteger()) return std::to_string(get<int>());
	if (isDouble()) return std::to_string(get<double>());
	if (isString()) return "\"" + get<std::string>() + "\"";

	throw std::runtime_error("Unknown Value type");
}