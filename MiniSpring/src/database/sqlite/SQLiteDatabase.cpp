#include"SQLiteDatabase.h"
#include"SQLiteQueryResult.h"
#include"SQLitePreparedStatement.h"

#include"SQLiteUtils.h"

#include"../DatabaseException.h"
#include"../DatabaseLock.h"

#include<iostream>
#include<string>
#include<vector>

#include<sqlite3.h>

SQLiteDatabase::SQLiteDatabase() : m_database(nullptr) {}

SQLiteDatabase::~SQLiteDatabase() {
	disconnect();
}

void SQLiteDatabase::connect(const std::string& connection) {
	int result = sqlite3_open(connection.c_str(), &m_database);

	if (result != SQLITE_OK) {
		std::string message = m_database ? sqlite3_errmsg(m_database) : "failed to open database";
		int code = m_database ? sqlite3_extended_errcode(m_database) : result;

		if (m_database) {
			sqlite3_close(m_database);
			m_database = nullptr;
		}

		throw DatabaseException(message, code, connection);
	}
}

void SQLiteDatabase::disconnect() {
	if (m_database) {
		sqlite3_close(m_database);
		m_database = nullptr;
	}
}

std::unique_ptr<QueryResult> SQLiteDatabase::execute(const std::string& sql) {
	if (!m_database) {
		throw DatabaseException("execute() called before connect() (or after disconnect())", 0, sql);
	}

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(m_database, sql.c_str(), -1, &statement, nullptr);
	if (result != SQLITE_OK) {
		throw DatabaseException(sqlite3_errmsg(m_database), sqlite3_extended_errcode(m_database), sql);
	}

	int columns = sqlite3_column_count(statement);

	std::vector<std::string> columnNames;
	for (int i = 0; i < columns; i++)
		columnNames.push_back(sqlite3_column_name(statement, i));

	auto rows = SQLiteUtils::readRows(statement);

	sqlite3_finalize(statement);

	return std::make_unique<SQLiteQueryResult>(std::move(rows), std::move(columnNames));
}

void SQLiteDatabase::beginTransaction() {
	execute("BEGIN TRANSACTION");
}

void SQLiteDatabase::commit() {
	execute("COMMIT");
}

void SQLiteDatabase::rollback() {
	execute("ROLLBACK");
}

std::unique_ptr<PreparedStatement> SQLiteDatabase::prepare(const std::string& sql) {
	return std::make_unique<SQLitePreparedStatement>(m_database, sql);
}

Value SQLiteDatabase::lastInsertId() {
	DatabaseGuard guard;
	return static_cast<int>(sqlite3_last_insert_rowid(m_database));
}