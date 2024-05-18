#pragma once

#include "java/value/JavaValue.hpp"

class JvFloat : public JavaValue {
  float value;

public:
  JvFloat(float value) : value(value) {}

  float asFloat() const override { return value; }
};
