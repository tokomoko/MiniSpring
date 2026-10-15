#include"MigrationManager.h"

#include"MigrationUtils.h"

#include"../DatabaseLock.h"

#include<fstream>
#include<sstream>
#include<iostream>
#include<algorithm>
#include<regex>

MigrationManager::MigrationManager(Database& database, const std::filesystem::path& path) : m_database(database), m_path(path) {}

void MigrationManager::createTable() {
	m_database.execute(R"(CREATE TABLE IF NOT EXISTS schema_version(version INTEGER PRIMARY KEY, name TEXT NOT NULL, checksum TEXT NOT NULL))");
}

void MigrationManager::migrate() {
	createTable();
	auto migrations = loadMigrations();

	for (const auto& migration : migrations) {
		if (!isApplied(migration)) {
			apply(migration);
		}
	}
}

Migration MigrationManager::parseMigrationFile(const std::filesystem::path& file) {
	std::string filename = file.filename().string();
	std::regex pattern(R"(V([0-9]+)__([a-zA-Z0-9_]+)\.sql)");
	std::smatch match;

	if (!std::regex_match(filename, match, pattern))
		throw std::runtime_error("Invalid migration filename: " + filename);
	
	int version = std::stoi(match[1].str());

	std::string name = match[2].str();
	std::ifstream input(file);

	if (!input.is_open())
		throw std::runtime_error("Cannot open migration: " + file.string());
	
	std::stringstream buffer;
	buffer << input.rdbuf();
	std::string content = buffer.str();

	if (content.size() >= 3 &&
		(unsigned char)content[0] == 0xEF &&
		(unsigned char)content[1] == 0xBB &&
		(unsigned char)content[2] == 0xBF) {
		content.erase(0, 3);
	}

	return Migration{ version, name, content, MigrationUtils::checksum(content) };
}

std::vector<Migration> MigrationManager::loadMigrations() {
	std::vector<Migration> migrations;
	if (!std::filesystem::exists(m_path))
		throw std::runtime_error("Migration directory does not exist: " + m_path.string());

	for (auto& entry : std::filesystem::directory_iterator(m_path)) {
		if (entry.path().extension() == ".sql") 
			migrations.push_back(parseMigrationFile(entry.path()));
	}

	//for (size_t i = 1; i < migrations.size(); i++) {
	//	if (migrations[i].version != migrations[i - 1].version + 1) {
	//		throw std::runtime_error("Migration version gap between " + std::to_string(migrations[i - 1].version) + " and " + std::to_string(migrations[i].version));
	//	}
	//}


	std::sort(migrations.begin(), migrations.end(), [](const Migration& a, const Migration& b) {
		return a.version < b.version;
	});

    for (size_t i = 1; i < migrations.size(); i++) {
        if (migrations[i].version == migrations[i - 1].version)
            throw std::runtime_error("Duplicate migration version " + std::to_string(migrations[i].version) + ": " + migrations[i - 1].name + " and " + migrations[i].name);
        if (migrations[i].version != migrations[i - 1].version + 1)
             throw std::runtime_error("Migration version gap between " + std::to_string(migrations[i - 1].version) + " and " + std::to_string(migrations[i].version));
    }

	return migrations;
}

bool MigrationManager::isApplied(const Migration& migration) {
	auto result = m_database.execute("SELECT checksum FROM schema_version WHERE version=" + std::to_string(migration.version));
	if (result->rowCount() == 0) return false;

	Value checksum = result->get(0, "checksum");
	if (checksum.get<std::string>() != migration.checksum) {
		throw std::runtime_error("Migration checksum mismatch: " + migration.name);
	}

	return true;
}

void MigrationManager::apply(const Migration& migration) {
	DatabaseGuard guard;

	std::cout << "Applying migration " << migration.version << ": " << migration.name << "\n";

	m_database.beginTransaction();

	try {
        m_database.executeScript(migration.sql);

		auto statement = m_database.prepare("INSERT INTO schema_version(version,name,checksum) VALUES(?,?,?)");
		statement->bind(0, migration.version);
		statement->bind(1, migration.name);
		statement->bind(2, migration.checksum);
		statement->executeUpdate();

		m_database.commit();
	}
	catch (...) {
		m_database.rollback();
		throw;
	}
}