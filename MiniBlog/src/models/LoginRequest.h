#pragma once

#include<MiniSpring.h>

#include <string>

struct LoginRequest {
	std::string username;
	std::string password;

	static EntityDescriptor<LoginRequest> descriptor() {
		TypeDescriptor<LoginRequest> type;
		type.addField(std::make_shared<FieldInfo<LoginRequest, int>>("username", &LoginRequest::username, FieldMetadata(FieldFlag::None)));
		type.addField(std::make_shared<FieldInfo<LoginRequest, std::string>>("username", &LoginRequest::password, FieldMetadata(FieldFlag::None)));
		return EntityDescriptor<LoginRequest>("users", type);
	}
};