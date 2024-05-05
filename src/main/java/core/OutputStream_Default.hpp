#pragma once

#include "code/Code.hpp"
#include "code/MethodReference.hpp"
#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "make.hpp"
#include "p.hpp"
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

class OutputStream_Default : public JavaClass {
public:
  std::string name() const override { return "core/OutputStream$Default"; }

  p<JavaObject> newObject(p<JavaClass>) const override {
    throw std::runtime_error(
      std::format("It is forbidden to create an instance of {}", name())
    );
  }

  p<Code> methodCode(MethodReference reference) const override {
    if (reference.equal("stdout", "()Lcore/OutputStream;"))
      return make<Code::Wrap>([](auto, auto) {
        std::cout << "ПРИВЕТ ВСЕМ ИЗ СТАВРАПОЛЯ!\n";
        return Code::ReturnVoid{};
      });
    throw std::runtime_error(
      std::format("Class {} has no method {}", name(), reference)
    );
  }
};
