#pragma once

#include<MiniSpring.h>

#include"../models/Post.h"

class PostRepository : public Repository<Post> {
public:
	using Repository<Post>::Repository;

	std::vector<Post> findNewest() {
		auto posts = findAll();
		std::sort(posts.begin(), posts.end(), [](auto& a, auto& b) {
			return a.createdAt > b.createdAt;
		});

		return posts;
	}

	std::optional<Post> findBySlug(std::string slug) {
		return findById(Value(slug));
	}
};