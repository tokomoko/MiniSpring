#pragma once

#include"../core/Core.h"

#include"FieldBase.h"
#include"FieldFlag.h"
#include"FieldMetadata.h"

template<typename Class, typename Type>
class FieldInfo : public FieldBase {
public:
	Type Class::* member;

	FieldInfo(std::string name, Type Class::* ptr, FieldMetadata metadata = FieldMetadata(FieldFlag::None)) : FieldBase(name, metadata), member(ptr) {}

	void set(void* object, Value value) override {
		Class* instance = static_cast<Class*>(object);
		instance->*member = value.get<Type>();
	}

	Value get(void* object) override {
		Class* instance = static_cast<Class*>(object);
		return Value(instance->*member);
	}
};