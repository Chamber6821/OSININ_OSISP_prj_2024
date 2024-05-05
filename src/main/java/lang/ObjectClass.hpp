#pragma once

#include "code/Code.hpp"
#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/object/JoMap.hpp"
#include "make.hpp"
#include "p.hpp"
#include <stdexcept>
#include <string>
#include <utility>

class ObjectClass : public JavaClass {
public:
  std::string name() const override { return "java/lang/Object"; }

  p<JavaObject> newObject(p<JavaClass> type) const override {
    return make<JoMap>(std::move(type));
  }

  p<Code> methodCode(std::string name, std::string signature) const override {
    if (name == "<init>" and signature == "()V")
      return make<Code::Wrap>([](auto, auto) { return Code::ReturnVoid{}; });
    throw std::runtime_error("Object has not methods");
  }
};
