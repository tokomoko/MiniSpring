#pragma once

#include <string>

class Post {
public:
	std::string id;
	std::string title;
	std::string content;
	int createdAt = 0;

	static EntityDescriptor<Post> descriptor() {
		TypeDescriptor<Post> type;

		type.addField(std::make_shared<FieldInfo<Post, std::string>>("id", &Post::id, FieldMetadata(FieldFlag::PrimaryKey)));
		type.addField(std::make_shared<FieldInfo<Post, std::string>>("title", &Post::title, FieldMetadata(FieldFlag::None)));
		type.addField(std::make_shared<FieldInfo<Post, std::string>>("content", &Post::content, FieldMetadata(FieldFlag::None)));
		type.addField(std::make_shared<FieldInfo<Post, int>>("createdAt", &Post::createdAt, FieldMetadata(FieldFlag::None)));

		return EntityDescriptor<Post>("posts", type);
	}
};