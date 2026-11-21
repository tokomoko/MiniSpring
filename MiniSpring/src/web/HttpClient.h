#pragma once

#include <string>
#include "../web/HttpResponse.h"

class MINISPRING_API HttpClient {
 public:
  static HttpResponse get(const std::string &host, int port,
                          const std::string &path);
};