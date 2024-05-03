#pragma once

#include "Interface.hpp"
#include "java/value/JavaValue.hpp"

class StackFrame : public Interface {
public:
  virtual p<JavaValue> pop() = 0;
  virtual void push(p<JavaValue> value) = 0;
};
