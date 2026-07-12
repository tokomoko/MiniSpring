#include"DatabaseConfiguration.h"

DatabaseConfiguration& DatabaseConfiguration::file(const std::string& file) {
	m_file = file;
	return *this;
}

DatabaseConfiguration& DatabaseConfiguration::migrations(const std::filesystem::path& directory) {
	m_migrations = directory;
	return *this;
}

const std::string& DatabaseConfiguration::file() const {
	return m_file;
}

const std::filesystem::path& DatabaseConfiguration::migrations() const {
	return m_migrations;
}