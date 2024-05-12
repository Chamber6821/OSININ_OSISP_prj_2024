#pragma once

#include "code/context/stack/StackFrame.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"
#include <cstdint>
#include <deque>
#include <format>
#include <iostream>
#include <ostream>
#include <variant>

class SfDiagnostic : public StackFrame {
  std::ostream &probe;
  p<StackFrame> origin;
  std::deque<std::string> content;

  struct Visitor {
    auto operator()(auto x) const { return std::format("{}", x); }

    auto operator()(p<JavaObject> o) const {
      return std::format("obj({:X})", std::intptr_t(o.get()));
    }
  };

  void print() const {
    std::print(probe, "[ ");
    for (auto &el : content) {
      std::print(probe, "{} ", el);
    }
    std::print(probe, "]\n");
  }

public:
  SfDiagnostic(std::ostream &probe, p<StackFrame> origin)
      : probe(probe), origin(std::move(origin)) {}

  p<JavaValue> pop() override {
    content.pop_back();
    print();
    return origin->pop();
  }

  void push(p<JavaValue> value) override {
    content.push_back(std::visit(Visitor{}, *value));
    print();
    origin->push(std::move(value));
  }
};
