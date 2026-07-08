#pragma once

#include "../../core/Value.h"

#include <sqlite3.h>
#include <vector>

namespace SQLiteUtils {
	using Row = std::vector<Value>;
	using Rows = std::vector<Row>;

	Rows readRows(sqlite3_stmt* statement);
}