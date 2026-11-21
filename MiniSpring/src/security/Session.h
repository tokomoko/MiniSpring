#pragma once

#include "../core/Core.h"
#include "../orm/EntityDescriptor.h"
#include "../reflection/FieldInfo.h"
#include "../reflection/TypeDescriptor.h"

#include <string>

struct MINISPRING_API Session {
  std::string id;
  std::string username;
  std::string authorities;
  int expiresAt = 0;

  static EntityDescriptor<Session> descriptor() {
    TypeDescriptor<Session> type;
    type.addField(std::make_shared<FieldInfo<Session, std::string>>(
        "id", &Session::id, FieldMetadata(FieldFlag::PrimaryKey)));
    type.addField(std::make_shared<FieldInfo<Session, std::string>>(
        "username", &Session::username, FieldMetadata(FieldFlag::None)));
    type.addField(std::make_shared<FieldInfo<Session, std::string>>(
        "authorities", &Session::authorities, FieldMetadata(FieldFlag::None)));
    type.addField(std::make_shared<FieldInfo<Session, int>>(
        "expires_at", &Session::expiresAt, FieldMetadata(FieldFlag::None)));
    return EntityDescriptor<Session>("sessions", type);
  }
};