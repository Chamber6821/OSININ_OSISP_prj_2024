#pragma once

#include "java/value/JavaValue.hpp"
#include <cstdint>

class JvInt : public JavaValue {
  std::int32_t value;

public:
  JvInt(std::int32_t value) : value(value) {}

  std::int32_t asInt() const override { return value; }
};
