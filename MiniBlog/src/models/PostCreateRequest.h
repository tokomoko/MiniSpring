#pragma once

#include<MiniSpring.h>

struct PostCreateRequest {
	std::string title;
	std::string content;

	static EntityDescriptor<PostCreateRequest> descriptor() {
		TypeDescriptor<PostCreateRequest> type;
		type.addField(std::make_shared<FieldInfo<PostCreateRequest, std::string>>("title", &PostCreateRequest::title, FieldMetadata(FieldFlag::None)));
		type.addField(std::make_shared<FieldInfo<PostCreateRequest, std::string>>("content", &PostCreateRequest::content, FieldMetadata(FieldFlag::None)));
		return EntityDescriptor<PostCreateRequest>("posts", type);
	}
};