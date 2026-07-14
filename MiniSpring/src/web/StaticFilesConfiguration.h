#pragma once

#include"../core/Core.h"

#include<filesystem>

class MINISPRING_API StaticFilesConfiguration {
public: 
	StaticFilesConfiguration& directory(const std::filesystem::path& path);

	const std::filesystem::path& directory() const;

private:
	std::filesystem::path m_directory;
};