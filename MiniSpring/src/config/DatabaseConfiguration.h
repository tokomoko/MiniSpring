#pragma once

#include"../core/Core.h"

#include<filesystem>

class MINISPRING_API DatabaseConfiguration {
public:
	DatabaseConfiguration& file(const std::string& file);
	DatabaseConfiguration& migrations(const std::filesystem::path& directory);
	const std::string& file() const;
	const std::filesystem::path& migrations() const;

private:
	std::string m_file;
	std::filesystem::path m_migrations;
};