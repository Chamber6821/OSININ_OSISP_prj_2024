#pragma once

#include "Interface.hpp"
#include "java/value/JavaValue.hpp"

class Locals : public Interface {
public:
  virtual p<JavaValue> at(int index) const = 0;
};
