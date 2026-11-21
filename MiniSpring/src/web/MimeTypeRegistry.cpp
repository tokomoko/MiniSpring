#include "MimeTypeRegistry.h"

MimeTypeRegistry::MimeTypeRegistry() {
  registerType(".html", "text/html");
  registerType(".css", "text/css");
  registerType(".js", "application/javascript");
  registerType(".png", "image/png");
  registerType(".jpg", "image/jpeg");
  registerType(".jpeg", "image/jpeg");
  registerType(".svg", "image/svg+xml");
  registerType(".json", "application/json");
}

std::string MimeTypeRegistry::get(const std::string &extension) const {
  auto it = m_types.find(extension);
  if (it != m_types.end()) {
    return it->second;
  }

  return "application/octet-stream";
}

void MimeTypeRegistry::registerType(const std::string &extension,
                                    const std::string &mimeType) {
  m_types[extension] = mimeType;
}