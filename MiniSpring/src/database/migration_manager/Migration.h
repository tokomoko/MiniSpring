#pragma once

#include <string>

struct Migration {
  int version;
  std::string name;
  std::string sql;
  std::string checksum;
};