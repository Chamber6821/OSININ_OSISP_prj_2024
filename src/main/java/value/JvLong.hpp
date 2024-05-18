#pragma once

#include "java/value/JavaValue.hpp"
#include <cstdint>

class JvLong : public JavaValue {
  std::int64_t value;

public:
  JvLong(std::int64_t value) : value(value) {}

  std::int64_t asLong() const override { return value; }
};
