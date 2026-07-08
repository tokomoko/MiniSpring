#pragma once

#include "../Database.h"
#include "Migration.h"

#include <filesystem>
#include <vector>

class MigrationManager {
public:
	MigrationManager(Database& database, const std::filesystem::path& path);

	void migrate();

private:
	void createTable();
	std::vector<Migration> loadMigrations();

	bool isApplied(const Migration& migration);
	void apply(const Migration& migration);
	
	Migration parseMigrationFile(const std::filesystem::path& file);

private:
	Database& m_database;
	std::filesystem::path m_path;
};