#pragma once

#include <string>

MINISPRING_API struct Migration {
	int version;
	std::string name;
	std::string sql;
	std::string checksum;
};