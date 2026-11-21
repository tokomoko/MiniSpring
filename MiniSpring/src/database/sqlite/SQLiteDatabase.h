#pragma once

#include "../../core/Core.h"
#include "../../core/Value.h"

#include "../Database.h"

class MINISPRING_API SQLiteDatabase : public Database {
 public:
  SQLiteDatabase();
  ~SQLiteDatabase();

  void connect(const std::string &connection) override;
  void disconnect() override;

  std::unique_ptr<QueryResult> execute(const std::string &sql) override;
  void executeScript(const std::string &sql) override;
  std::unique_ptr<PreparedStatement> prepare(const std::string &sql) override;

  void beginTransaction() override;
  void commit() override;
  void rollback() override;
  Value lastInsertId() override;

 private:
  struct sqlite3 *m_database;
};