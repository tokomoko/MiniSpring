#include "Json.h"

JsonValue& Json::operator[](const std::string& key) {
	return m_object[key];
}

std::string Json::stringify() {
	std::string result = "{";
	bool first = true;

	for (const auto& pair : m_object) {
		if (!first) result += ",";

		result += "\"";
		result += pair.first;
		result += "\":";


		result += pair.second.stringify();
		first = false;
	}

	result += "}";

	return result;
}

JsonObject Json::asObject() const {
	return m_object;
}