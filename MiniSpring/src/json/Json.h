#pragma once

#include <string>
#include <unordered_map>

#include"JsonValue.h"

class Json {
public:
	std::string stringify();

	JsonValue& operator[](const std::string& key);

	JsonObject asObject() const;

private:
	JsonObject m_object;
};