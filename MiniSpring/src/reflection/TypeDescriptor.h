#pragma once

template<typename T>
class TypeDescriptor {
public:
	using RegisterFunction = std::function<void(TypeDescriptor&)>;
	std::vector<std::shared_ptr<FieldBase>> fields;

	void addField(std::shared_ptr<FieldBase> field) {
		fields.push_back(field);
	}
};