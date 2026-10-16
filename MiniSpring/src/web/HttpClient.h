#pragma once

#include "../web/HttpResponse.h"
#include <string>

class MINISPRING_API HttpClient {
public:
    static HttpResponse get(const std::string& host, int port, const std::string& path);
};