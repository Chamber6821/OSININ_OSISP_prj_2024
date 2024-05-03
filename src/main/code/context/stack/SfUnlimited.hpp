#pragma once

#include "code/context/stack/StackFrame.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"
#include <stack>

class SfUnlimited : public StackFrame {
  std::stack<p<JavaValue>> stack;

public:
  p<JavaValue> pop() override {
    auto val = stack.top();
    stack.pop();
    return val;
  }

  void push(p<JavaValue> value) override { stack.push(std::move(value)); }
};
