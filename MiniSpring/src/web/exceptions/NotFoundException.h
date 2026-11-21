#pragma once

#include "../../core/Core.h"

#include "HttpException.h"

class MINISPRING_API NotFoundException : public HttpException {
 public:
  NotFoundException(std::string message)
      : HttpException(404, std::move(message)) {}
};