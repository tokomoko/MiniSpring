#pragma once

#include<MiniSpring.h>

#include <string>

struct PostUpdateRequest {
	std::string title;
	std::string content;

	static EntityDescriptor<PostUpdateRequest> descriptor() {
		TypeDescriptor<PostUpdateRequest> type;
		type.addField(std::make_shared<FieldInfo<PostUpdateRequest, std::string>>("title", &PostUpdateRequest::title, FieldMetadata(FieldFlag::None)));
		type.addField(std::make_shared<FieldInfo<PostUpdateRequest, std::string>>("content", &PostUpdateRequest::content, FieldMetadata(FieldFlag::None)));
		return EntityDescriptor<PostUpdateRequest>("posts", type);
	}
};