#pragma once

#include<MiniSpring.h>

struct TodoUpdateRequest {
	bool completed;

	static EntityDescriptor<TodoUpdateRequest> descriptor() {
		TypeDescriptor<TodoUpdateRequest> type;
		type.addField(std::make_shared<FieldInfo<TodoUpdateRequest, bool>>("completed", &TodoUpdateRequest::completed, FieldMetadata(FieldFlag::None)));
		return EntityDescriptor<TodoUpdateRequest>("todos", type);
	}
};
