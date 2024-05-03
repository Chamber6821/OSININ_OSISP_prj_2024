#pragma once

#include "Interface.hpp"
#include "code/context/instruction-pointer/InstructionPointer.hpp"
#include "code/context/stack/StackFrame.hpp"
#include "java/value/JavaValues.hpp"
#include "p.hpp"

class Context : public Interface {
public:
  virtual p<InstructionPointer> instructionPointer() const = 0;
  virtual p<StackFrame> stack() const = 0;
  virtual p<JavaValues> locals() const = 0;
  virtual p<JavaValues> arguments() const = 0;
};
