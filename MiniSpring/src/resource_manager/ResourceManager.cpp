#include"ResourceManager.h"
#include"ResourceException.h"

#include<fstream>
#include<sstream>
#include<stdexcept>

ResourceManager::ResourceManager(std::filesystem::path root) {
	m_root = std::filesystem::weakly_canonical(root);
}

bool ResourceManager::exists(const std::string& path) const {
	auto fullPath = m_root / path;
	if (!isSafePath(fullPath)) return false;
	return std::filesystem::exists(fullPath);
}

std::string ResourceManager::read(const std::string& path) const {
	auto fullPath = m_root / path;
	if (!isSafePath(fullPath)) throw ResourceException(ResourceException::Type::InvalidPath,"Invalid resource path");
	if (!std::filesystem::exists(fullPath)) throw ResourceException(ResourceException::Type::NotFound, "Resource not found");

	std::ifstream file(fullPath, std::ios::binary);

	if (!file.is_open()) throw ResourceException(ResourceException::Type::ReadError, "Cannot open resource");

	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}

bool ResourceManager::isSafePath(const std::filesystem::path& path) const {
	auto normalized = std::filesystem::weakly_canonical(path);
	auto root = m_root.string();
	auto target = normalized.string();

	return target.compare(0, root.size(), root) == 0;
}