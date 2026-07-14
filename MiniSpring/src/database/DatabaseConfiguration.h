#pragma once

#include"../core/Core.h"

#include<filesystem>

class MINISPRING_API DatabaseConfiguration {
public:
	DatabaseConfiguration& file(const std::filesystem::path& file);
	DatabaseConfiguration& migrations(const std::filesystem::path& directory);
	const std::filesystem::path& file() const;
	const std::filesystem::path& migrations() const;

private:
	std::filesystem::path m_file;
	std::filesystem::path m_migrations;
};