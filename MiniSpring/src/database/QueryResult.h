#pragma once

#include"../core/Value.h"

#include<vector>

class QueryResult {
public:
	virtual ~QueryResult() = default;

	virtual int rowCount() const = 0;
	virtual int columnCount() const = 0;

	virtual Value get(int row, const std::string& column) const = 0;
	virtual std::vector<std::string> columns() const = 0;
};