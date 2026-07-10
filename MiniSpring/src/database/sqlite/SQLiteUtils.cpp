#include "SQLiteUtils.h"

#include"../DatabaseException.h"

namespace SQLiteUtils {
	MINISPRING_API Rows readRows(sqlite3_stmt* statement) {
		Rows rows;
		int columns = sqlite3_column_count(statement);

		while (true) {
			int result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				Row row;
				for (int i = 0; i < columns; i++) {
					int type = sqlite3_column_type(statement, i);
					switch (type) {
					case SQLITE_INTEGER: row.emplace_back(sqlite3_column_int(statement, i)); break;
					case SQLITE_FLOAT: row.emplace_back(sqlite3_column_double(statement, i)); break;
					case SQLITE_TEXT: row.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, i))); break;
					case SQLITE_NULL: row.emplace_back(nullptr); break;
					default: row.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, i)));
					}
				}

				rows.push_back(std::move(row));
			}
			else if (result == SQLITE_DONE) {
				break;
			}
			else {
				sqlite3* db = sqlite3_db_handle(statement);
				throw DatabaseException(sqlite3_errmsg(db), sqlite3_extended_errcode(db), "");
			}
		}

		return rows;
	}

}