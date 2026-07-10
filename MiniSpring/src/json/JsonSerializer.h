#pragma once

#include"../core/Core.h"

#include "../reflection/TypeDescriptor.h"
#include "JsonValue.h"

class MINISPRING_API JsonSerializer {
public:

	template<typename T>
	static JsonValue serialize(const T& object, const TypeDescriptor<T>& type) {
		JsonObject json;
		for (auto& field : type.fields)
			json[field->name()] = field->get((void*)&object);

		return JsonValue(json);
	}

	template<typename T>
	static JsonValue serialize(const std::vector<T>& objects, const TypeDescriptor<T>& type) {
		JsonArray array;
		for (const auto& object : objects) {
			array.push_back(serialize(object, type));
		}

		return JsonValue(array);
	}

	template<typename T>
	static T deserialize(const JsonValue& json, const TypeDescriptor<T>& type) {
		T object;
		const auto& values = json.as<JsonObject>();

		for (auto& field : type.fields) {
			auto it = values.find(field->name());
			if (it == values.end()) continue;

			field->set(&object, it->second.as<Value>());
		}

		return object;
	}
};