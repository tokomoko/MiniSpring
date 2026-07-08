#pragma once

#include"../PreparedStatement.h"

#include<sqlite3.h>

class SQLitePreparedStatement : public PreparedStatement {
public:

	SQLitePreparedStatement(sqlite3* database, const std::string& sql);
	~SQLitePreparedStatement();

	void bind(int index, int value) override;
	void bind(int index, double value) override;
	void bind(int index, const std::string& value) override;
	virtual void bind(int index, const char* value) override;
	void bind(int index, bool value) override;
	void bind(int index, const Value& value) override;

	std::unique_ptr<QueryResult> execute() override;

	void executeUpdate() override;

private:
	sqlite3_stmt* m_statement;
	std::string m_sql;
};