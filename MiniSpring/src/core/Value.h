#pragma once

#include"Core.h"

#include<variant>
#include<string>
#include<stdexcept>
#include<iostream>

class MINISPRING_API Value {
	using Type = std::variant<std::nullptr_t, bool, int, double, std::string>;
public:
	Value();
	Value(std::nullptr_t value);
	Value(bool value);
	Value(int value);
	Value(double value);
	Value(const char* value);
	Value(std::string value);

	std::string escape(const std::string& s) const;

	const Type& data() const {
		return m_value;
	}

	Type& data() {
		return m_value;
	}

	template<typename T>
	T get() const {
		
		if constexpr (std::is_same_v<T, bool>) {
			if (is<int>()) return std::get<int>(m_value) != 0;
		}

		if constexpr (std::is_same_v<T, int>) {
			if (is<bool>()) return std::get<bool>(m_value) ? 1 : 0;
		}

		if (!is<T>()) throw std::runtime_error("Invalid Value conversion");
		
		return std::get<T>(m_value);
	}

	template<typename T>
	bool is() const {
		return std::holds_alternative<T>(m_value);
	}

	bool isNull() const {
		return is<std::nullptr_t>();
	}

	bool isBoolean() const {
		return is<bool>();
	}

	bool isInteger() const {
		return is<int>();
	}

	bool isDouble() const {
		return is<double>();
	}

	bool isNumber() const {
		return is<int>() || is<double>();
	}

	bool isString() const {
		return is<std::string>();
	}

	std::string toString() const;
	std::string toJsonString() const;
	std::string typeName() const;

private:
	Type m_value;
};