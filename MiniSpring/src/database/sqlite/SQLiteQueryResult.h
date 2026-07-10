#pragma once

#include"../../core/Core.h"

#include "../QueryResult.h"

#include <vector>
#include <string>
#include <unordered_map>

class MINISPRING_API SQLiteQueryResult : public QueryResult {
public:
	SQLiteQueryResult(std::vector<std::vector<Value>> data, std::vector<std::string> columns) : m_data(std::move(data)), m_columns(std::move(columns)) {
		for (int i = 0; i < m_columns.size(); i++) {
			m_columnIndexes[m_columns[i]] = i;
		}
	}

	int rowCount() const override {
		return static_cast<int>(m_data.size());
	}

	int columnCount() const override {
		return static_cast<int>(m_columns.size());
	}

	Value get(int row, const std::string& column) const override {
		auto it = m_columnIndexes.find(column);

		if (it == m_columnIndexes.end()) throw std::runtime_error("Column not found: " + column);

		return m_data[row][it->second];
	}

	std::vector<std::string> columns() const override {
		return m_columns;
	}

private:
	std::vector<std::vector<Value>> m_data;
	std::vector<std::string> m_columns;
	std::unordered_map<std::string, int> m_columnIndexes;
};