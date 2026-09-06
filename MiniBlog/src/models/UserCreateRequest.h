#pragma once

#include<MiniSpring.h>

#include<string>

struct UserCreateRequest {
	std::string username;
	std::string password;
	bool admin = false;

	static EntityDescriptor<UserCreateRequest> descriptor() {
		TypeDescriptor<UserCreateRequest> type;
		type.addField(std::make_shared<FieldInfo<UserCreateRequest, std::string>>("username", &UserCreateRequest::username, FieldMetadata(FieldFlag::None)));
		type.addField(std::make_shared<FieldInfo<UserCreateRequest, std::string>>("password", &UserCreateRequest::password, FieldMetadata(FieldFlag::None)));
		type.addField(std::make_shared<FieldInfo<UserCreateRequest, bool>>("admin", &UserCreateRequest::admin, FieldMetadata(FieldFlag::None)));
		return EntityDescriptor<UserCreateRequest>("users", type);
	}
};