#pragma once

#include"../json/JsonValue.h"

#include<string>

class FieldBase {
public:
	std::string name;

	FieldBase(std::string n) : name(n) {}

	virtual void set(void* object, JsonValue value) = 0;
	virtual JsonValue get(void* object) = 0;
	virtual ~FieldBase() {}
};