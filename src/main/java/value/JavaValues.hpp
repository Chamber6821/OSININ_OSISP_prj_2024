#pragma once

#include "Interface.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"

class JavaValues : public Interface {
public:
  virtual p<JavaValue> at(int index) const = 0;
  virtual void put(int index, p<JavaValue> value) = 0;
};
