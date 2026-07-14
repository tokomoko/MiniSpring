#pragma once

#include"../repositories/PostRepository.h"

#include"../models/PostCreateRequest.h"
#include"../models/PostUpdateRequest.h"

#include<algorithm>
#include<cctype>
#include<ctime>
#include<fstream>
#include<sstream>

class PostService {
public:
	PostService(PostRepository& repository) : m_repository(repository) {}

	std::vector<Post> getAllPosts() {
		return m_repository.findNewest();
	}

	std::optional<Post> getPost(std::string id) {
		return m_repository.findBySlug(id);
	}

	Post create(PostCreateRequest request) {
		Post post;

		post.id = uniqueSlug(slugify(request.title));
		post.title = request.title;
		post.content = request.content;
		post.createdAt = std::time(nullptr);

		m_repository.save(post);

		return post;
	}

	Post update(std::string id, PostUpdateRequest request) {
		auto post = m_repository.findBySlug(id);

		if (!post) throw std::runtime_error("Post not found");

		if (!request.title.empty())
			post->title = request.title;

		post->content = request.content;
		m_repository.update(post.value());

		return post.value();
	}

	void deletePost(std::string id)
	{
		m_repository.removeById(Value(id));
	}

private:
	std::string slugify(const std::string& title) {
		std::string slug;
		slug.reserve(title.size());

		for (unsigned char c : title) {
			if (c >= 0x80) {
				slug += static_cast<char>(c);
			}
			else if (std::isalnum(c)) {
				slug += static_cast<char>(std::tolower(c));
			}
			else if (!slug.empty() && slug.back() != '-') {
				slug += '-';
			}
		}

		while (!slug.empty() && slug.back() == '-') {
			slug.pop_back();
		}

		return slug.empty() ? "post" : slug;
	}

	std::string uniqueSlug(const std::string& base) {
		std::string slug = base;
		int suffix = 2;

		while (m_repository.findBySlug(slug)) {
			slug = base + "-" + std::to_string(suffix);
			suffix++;
		}

		return slug;
	}

private:
	PostRepository& m_repository;
};