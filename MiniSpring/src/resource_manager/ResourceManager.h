#pragma once

#include <filesystem>
#include <string>

class ResourceManager {
public:
	explicit ResourceManager(std::filesystem::path root);

	std::string read(const std::string& path) const;
	bool exists(const std::string& path) const;

private:
	bool isSafePath(const std::filesystem::path& path) const;

private:
	std::filesystem::path m_root;
};