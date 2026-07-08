#pragma once

template<typename Class, typename Type>
class Field {
public:
	std::string name;
	Type Class::* member;

	Field(std::string fieldName, Type Class::* ptr) : name(fieldName), member(ptr) {}

	Type& get(Class& object) {
		return object.*member;
	}
};