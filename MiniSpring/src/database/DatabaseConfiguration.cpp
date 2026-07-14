#include"DatabaseConfiguration.h"

DatabaseConfiguration& DatabaseConfiguration::file(const std::filesystem::path& file) {
	m_file = file;
	return *this;
}

DatabaseConfiguration& DatabaseConfiguration::migrations(const std::filesystem::path& directory) {
	m_migrations = directory;
	return *this;
}

const std::filesystem::path& DatabaseConfiguration::file() const {
	return m_file;
}

const std::filesystem::path& DatabaseConfiguration::migrations() const {
	return m_migrations;
}