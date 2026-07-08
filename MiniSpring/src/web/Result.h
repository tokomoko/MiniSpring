#pragma once

#include <variant>

template<typename T, typename E>
class Result {
public:
	static Result success(T value) {
		return Result(value, true);
	}

	static Result error(E error) {
		return Result(error, false);
	}

	bool isOk() const {
		return m_success;
	}

	T& value() {
		return std::get<T>(m_data);
	}

	E& error() {
		return std::get<E>(m_data);
	}

private:
	Result(T value, bool success) : m_data(value), m_success(success) {}
	Result(E error, bool success) : m_data(error), m_success(success) {}

private:

	std::variant<T, E> m_data;

	bool m_success;

};