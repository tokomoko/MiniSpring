#pragma once

#include"../../core/Core.h"

#include "../../core/Value.h"

#include <sqlite3.h>
#include <vector>

namespace SQLiteUtils {
	using Row = std::vector<Value>;
	using Rows = std::vector<Row>;

	MINISPRING_API Rows readRows(sqlite3_stmt* statement);
}