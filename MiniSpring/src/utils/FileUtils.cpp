#include "FileUtils.h"

#include<string>

std::string extension(const std::string& path) {
	auto pos = path.find_last_of('.');
	if (pos == std::string::npos) return "";
	return path.substr(pos);
}