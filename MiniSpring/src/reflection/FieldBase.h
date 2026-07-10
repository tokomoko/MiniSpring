#pragma once

#include"FieldMetadata.h"

#include"../core/Core.h"
#include"../core/Value.h"

#include<string>

class MINISPRING_API FieldBase {
public:
	FieldBase(std::string n, FieldMetadata metadata) : m_name(n), m_metadata(metadata) {}

	virtual void set(void* object, Value value) = 0;
	virtual Value get(void* object) = 0;
	virtual ~FieldBase() {}

	const std::string& name() const {
		return m_name;
	}

	const FieldMetadata& metadata() const {
		return m_metadata;
	}

protected:
	std::string m_name;
	FieldMetadata m_metadata;
};