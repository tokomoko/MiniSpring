#pragma once

#include<MiniSpring.h>

#include<string>

struct Session {
	std::string id;
	int userId;
	long long expiresAt;

	static EntityDescriptor<Session> descriptor() {
		TypeDescriptor<Session> type;
		type.addField(std::make_shared<FieldInfo<Session, std::string>>("username", &Session::id, FieldMetadata(FieldFlag::None)));
		type.addField(std::make_shared<FieldInfo<Session, std::string>>("password_hash", &Session::userId, FieldMetadata(FieldFlag::None)));
		type.addField(std::make_shared<FieldInfo<Session, bool>>("admin", &Session::expiresAt, FieldMetadata(FieldFlag::None)));
		return EntityDescriptor<Session>("users", type);
	}
};