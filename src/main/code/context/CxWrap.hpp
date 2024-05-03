#pragma once

#include "code/context/Context.hpp"
#include "code/context/stack/StackFrame.hpp"
#include "java/value/JavaValues.hpp"
#include "p.hpp"
#include <cstdint>

class CxWrap : public Context {
  std::uint32_t _address;
  p<StackFrame> _stack;
  p<JavaValues> _locals;
  p<JavaValues> _arguments;

public:
  CxWrap(
    std::uint32_t address, p<StackFrame> stack, p<JavaValues> locals,
    p<JavaValues> arguments
  )
      : _address(address), _stack(std::move(stack)), _locals(std::move(locals)),
        _arguments(std::move(arguments)) {}

  std::uint32_t executionPoint() const override { return _address; }

  p<StackFrame> stack() const override { return _stack; }

  p<JavaValues> locals() const override { return _locals; }

  p<JavaValues> arguments() const override { return _arguments; }
};
