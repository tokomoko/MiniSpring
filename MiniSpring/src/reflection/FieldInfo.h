#pragma once

#include"FieldBase.h"

template<typename Class, typename Type>
class FieldInfo : public FieldBase {
public:
	Type Class::* member;

	FieldInfo(std::string name, Type Class::* ptr) : FieldBase(name), member(ptr) {}

	void set(void* object, JsonValue value) override {
		Class* instance = static_cast<Class*>(object);
		instance->*member = value.template as<Type>();
	}

	JsonValue get(void* object) override {
		Class* instance = static_cast<Class*>(object);
		return JsonValue(instance->*member);
	}
};