#pragma once

class EntityDescriptorBase {
public:
	virtual ~EntityDescriptorBase() = default;

	virtual FieldBase& primaryKey() = 0;
};