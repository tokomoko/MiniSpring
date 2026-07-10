#pragma once

#include<MiniSpring.h>

struct TodoUpdateRequest {
	bool completed;
};

inline void registerTodoUpdateRequest(EntityRegistry& registry) {
	TypeDescriptor<TodoUpdateRequest> type;
	type.addField(std::make_shared<FieldInfo<TodoUpdateRequest, bool>>("completed", &TodoUpdateRequest::completed, FieldMetadata(FieldFlag::None)));
	EntityDescriptor<TodoUpdateRequest> entity("todos", type);

	registry.registerEntity(entity);
}