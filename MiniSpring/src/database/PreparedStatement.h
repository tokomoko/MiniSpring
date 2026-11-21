#pragma once

#include "../core/Core.h"
#include "../core/Value.h"
#include "QueryResult.h"

#include <string>

class MINISPRING_API PreparedStatement {
 public:
  virtual ~PreparedStatement() = default;

  virtual void bind(int index, int value) = 0;
  virtual void bind(int index, double value) = 0;
  virtual void bind(int index, const std::string &value) = 0;
  virtual void bind(int index, const char *value) = 0;
  virtual void bind(int index, bool value) = 0;
  virtual void bind(int index, const Value &value) = 0;

  virtual std::unique_ptr<QueryResult> execute() = 0;

  virtual void executeUpdate() = 0;
};