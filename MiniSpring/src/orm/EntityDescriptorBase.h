#pragma once

#include "../core/Core.h"

#include "../reflection/FieldBase.h"

class MINISPRING_API EntityDescriptorBase {
 public:
  virtual ~EntityDescriptorBase() = default;

  virtual FieldBase &primaryKey() = 0;
};