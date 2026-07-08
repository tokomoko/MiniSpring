#pragma once

#include "../Database.h"

class SQLiteDatabase : public Database {
public:
	SQLiteDatabase();
	~SQLiteDatabase();

	void connect(const std::string& connection) override;
	void disconnect() override;

	std::unique_ptr<QueryResult> execute(const std::string& sql) override;
	std::unique_ptr<PreparedStatement> prepare(const std::string& sql) override;

	void beginTransaction() override;
	void commit() override;
	void rollback() override;

private:
	struct sqlite3* m_database;
};