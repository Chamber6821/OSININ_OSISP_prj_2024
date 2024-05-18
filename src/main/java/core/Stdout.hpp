#pragma once

#include "code/Code.hpp"
#include "code/context/Context.hpp"
#include "java/class/JavaClass.hpp"
#include "make.hpp"
#include "p.hpp"
#include <ostream>

class Stdout : public JavaClass {
  std::ostream &out;
  p<JavaClass> super;

public:
  Stdout(std::ostream &out, p<JavaClass> super)
      : out(out), super(std::move(super)) {}

  std::string name() const override { return "core/Stdout"; }

  p<JavaObject> newObject(p<JavaClass> type) const override {
    return super->newObject(std::move(type));
  }

  p<Code> methodCode(MethodReference reference) const override {
    if (reference.equal("put", "(Ljava/lang/String;)V"))
      return make<Code::Wrap>([this](p<Context> context, auto) {
        auto stringObject = context->locals()->at(1)->asObject();
        auto arrayObject = stringObject->field("$content")->asObject();
        auto length = arrayObject->field("$length")->asInt();
        auto string = std::string(length, ' ');
        for (int i = 0; i < length; i++) {
          string[i] = arrayObject->field(std::format("${}", i))->asInt();
        }
        out << string;
        return Code::ReturnVoid{};
      });
    throw std::runtime_error(
      std::format("Class {} has no method {}", name(), reference)
    );
  }
};
