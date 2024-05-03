#pragma once

#include "Interface.hpp"
#include "code/context/stack/StackFrame.hpp"
#include "java/value/JavaValues.hpp"
#include <cstdint>

class Context : public Interface {
public:
  virtual std::uint32_t executionPoint() const = 0;
  virtual p<StackFrame> stack() const = 0;
  virtual p<JavaValues> locals() const = 0;
  virtual p<JavaValues> arguments() const = 0;
};
