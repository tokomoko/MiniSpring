#pragma once

#include<string>
#include<vector>

struct RouteMetadata {
	bool requiresAuth = false;
	std::vector<std::string> requiredAuthorities;
};