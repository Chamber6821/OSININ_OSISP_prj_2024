#pragma once

#include "code/Code.hpp"
#include "code/MethodReference.hpp"
#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/object/JoMap.hpp"
#include "make.hpp"
#include "p.hpp"
#include <format>
#include <stdexcept>
#include <string>
#include <utility>

class ObjectClass : public JavaClass {
public:
  std::string name() const override { return "java/lang/Object"; }

  p<JavaObject> newObject(p<JavaClass> type) const override {
    return make<JoMap>(std::move(type));
  }

  p<Code> methodCode(MethodReference reference) const override {
    if (reference.equal("<init>", "()V"))
      return make<Code::Wrap>([](auto, auto) { return Code::ReturnVoid{}; });
    throw std::runtime_error(std::format("Class has no method {}", reference));
  }
};
