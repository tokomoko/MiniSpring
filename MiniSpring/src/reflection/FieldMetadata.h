#pragma once

#include"FieldFlag.h"

//struct FieldMetadata {
//	bool primaryKey = false;
//};

class FieldMetadata
{
public:

	FieldMetadata(FieldFlag flags = FieldFlag::None) : m_flags(flags) {}

	bool has(FieldFlag flag) const {
		return (m_flags & flag) != FieldFlag::None;
	}

	void add(FieldFlag flag) {
		m_flags |= flag;
	}

	void remove(FieldFlag flag) {
		m_flags &= ~flag;
	}

	FieldFlag flags() const {
		return m_flags;
	}

private:
	FieldFlag m_flags;
};