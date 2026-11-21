#include "MigrationUtils.h"

#include <functional>
#include <sstream>

namespace MigrationUtils {
MINISPRING_API std::string checksum(const std::string &text) {
  std::hash<std::string> hash;
  size_t value = hash(text);
  std::stringstream stream;
  stream << value;
  return stream.str();
}
}  // namespace MigrationUtils