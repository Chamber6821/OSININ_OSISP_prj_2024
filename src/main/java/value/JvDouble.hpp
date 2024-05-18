#pragma once

#include "java/value/JavaValue.hpp"

class JvDouble : public JavaValue {
  double value;

public:
  JvDouble(double value) : value(value) {}

  double asDouble() const override { return value; }
};
