#pragma once

#include "code/context/Context.hpp"
#include "code/context/instruction-pointer/InstructionPointer.hpp"
#include "code/context/stack/StackFrame.hpp"
#include "java/value/JavaValues.hpp"
#include "p.hpp"
#include <utility>

class CxWrap : public Context {
  p<InstructionPointer> _instructionPointer;
  p<StackFrame> _stack;
  p<JavaValues> _locals;
  p<JavaValues> _arguments;

public:
  CxWrap(
    p<InstructionPointer> instructionPointer, p<StackFrame> stack,
    p<JavaValues> locals, p<JavaValues> arguments
  )
      : _instructionPointer(std::move(instructionPointer)),
        _stack(std::move(stack)), _locals(std::move(locals)),
        _arguments(std::move(arguments)) {}

  p<InstructionPointer> instructionPointer() const override {
    return _instructionPointer;
  }

  p<StackFrame> stack() const override { return _stack; }

  p<JavaValues> locals() const override { return _locals; }

  p<JavaValues> arguments() const override { return _arguments; }
};
