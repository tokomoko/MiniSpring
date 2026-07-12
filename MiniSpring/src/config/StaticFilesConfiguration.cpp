#include"StaticFilesConfiguration.h"

StaticFilesConfiguration& StaticFilesConfiguration::directory(const std::filesystem::path& path) {
	m_directory = path;
	return *this;
}

const std::filesystem::path& StaticFilesConfiguration::directory() const {
	return m_directory;
}