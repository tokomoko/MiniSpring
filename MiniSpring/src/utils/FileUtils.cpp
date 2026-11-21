#include "FileUtils.h"

#include "../core/Core.h"

#include <string>

MINISPRING_API std::string extension(const std::string &path) {
  auto pos = path.find_last_of('.');
  if (pos == std::string::npos) return "";
  return path.substr(pos);
}