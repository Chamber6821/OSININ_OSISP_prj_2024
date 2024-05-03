#pragma once

#include "Interface.hpp"
#include "code/context/locals/Locals.hpp"
#include "code/context/stack/StackFrame.hpp"
#include <cstdint>

class Context : public Interface {
public:
  virtual std::uint64_t executionPoint() const = 0;
  virtual p<StackFrame> stack() const = 0;
  virtual p<Locals> locals() const = 0;
  virtual p<Locals> arguments() const = 0;
};
