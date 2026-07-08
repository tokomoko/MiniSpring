#pragma once

#include <memory>
#include <string>

#include"QueryResult.h"
#include"PreparedStatement.h"

class Database {
public:
	virtual ~Database() = default;

	virtual void connect(const std::string& connectionString) = 0;
	virtual void disconnect() = 0;

	virtual std::unique_ptr<QueryResult> execute(const std::string& sql) = 0;
	virtual std::unique_ptr<PreparedStatement> prepare(const std::string& sql) = 0;

	virtual void beginTransaction() = 0;
	virtual void commit() = 0;
	virtual void rollback() = 0;
};