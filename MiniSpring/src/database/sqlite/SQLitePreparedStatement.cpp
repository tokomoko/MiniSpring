#include "SQLitePreparedStatement.h"
#include "SQLiteQueryResult.h"

#include "SQLiteUtils.h"

#include "../DatabaseException.h"

#include "../../core/Value.h"

#include <string>
#include <vector>

#include <sqlite3.h>

SQLitePreparedStatement::SQLitePreparedStatement(sqlite3 *database,
                                                 const std::string &sql)
    : m_statement(nullptr), m_sql(sql) {
  int result =
      sqlite3_prepare_v2(database, sql.c_str(), -1, &m_statement, nullptr);
  if (result != SQLITE_OK)
    throw DatabaseException(sqlite3_errmsg(database),
                            sqlite3_extended_errcode(database), sql);
}

SQLitePreparedStatement::~SQLitePreparedStatement() {
  sqlite3_finalize(m_statement);
}

void SQLitePreparedStatement::bind(int index, const Value &value) {
  int result = SQLITE_ERROR;

  if (value.isNull())
    result = sqlite3_bind_null(m_statement, index + 1);
  else if (value.is<int>())
    result = sqlite3_bind_int(m_statement, index + 1, value.get<int>());
  else if (value.is<double>())
    result = sqlite3_bind_double(m_statement, index + 1, value.get<double>());
  else if (value.is<bool>())
    result =
        sqlite3_bind_int(m_statement, index + 1, value.get<bool>() ? 1 : 0);
  else if (value.is<std::string>())
    result = sqlite3_bind_text(m_statement, index + 1,
                               value.get<std::string>().c_str(), -1,
                               SQLITE_TRANSIENT);

  if (result != SQLITE_OK) {
    sqlite3 *db = sqlite3_db_handle(m_statement);
    throw DatabaseException(sqlite3_errmsg(db), sqlite3_extended_errcode(db),
                            m_sql);
  }
}

void SQLitePreparedStatement::bind(int index, int value) {
  bind(index, Value(value));
}

void SQLitePreparedStatement::bind(int index, double value) {
  bind(index, Value(value));
}

void SQLitePreparedStatement::bind(int index, const std::string &value) {
  bind(index, Value(value));
}

void SQLitePreparedStatement::bind(int index, const char *value) {
  bind(index, Value(std::string(value)));
}

void SQLitePreparedStatement::bind(int index, bool value) {
  bind(index, Value(value));
}

void SQLitePreparedStatement::executeUpdate() {
  int result = sqlite3_step(m_statement);

  if (result != SQLITE_DONE) {
    sqlite3 *db = sqlite3_db_handle(m_statement);
    std::string message = sqlite3_errmsg(db);
    int code = sqlite3_extended_errcode(db);
    sqlite3_reset(m_statement);
    throw DatabaseException(message, code, m_sql);
  }

  sqlite3_reset(m_statement);
}

std::unique_ptr<QueryResult> SQLitePreparedStatement::execute() {
  int columns = sqlite3_column_count(m_statement);
  auto rows = SQLiteUtils::readRows(m_statement);

  std::vector<std::string> columnNames;
  for (int i = 0; i < columns; i++)
    columnNames.push_back(sqlite3_column_name(m_statement, i));

  sqlite3_reset(m_statement);

  return std::make_unique<SQLiteQueryResult>(std::move(rows),
                                             std::move(columnNames));
}