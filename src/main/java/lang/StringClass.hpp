#pragma once

#include "code/Code.hpp"
#include "code/MethodReference.hpp"
#include "code/context/Context.hpp"
#include "java/object/JavaObject.hpp"
#include "make.hpp"
#include "p.hpp"
#include <string>
#include <utility>

class StringClass : public JavaClass {
  p<JavaClass> super;

public:
  StringClass(p<JavaClass> super) : super(std::move(super)) {}

  std::string name() const override { return "java/lang/String"; }

  p<JavaObject> newObject(p<JavaClass> type) const override {
    return super->newObject(type);
  }

  p<Code> methodCode(MethodReference reference) const override {
    if (reference.equal("<init>", "([B)V"))
      return make<Code::Wrap>([&](p<Context> context, auto) {
        auto arguments = context->locals();
        auto arg0 = arguments->at(0);
        auto arg1 = arguments->at(1);
        std::get<p<JavaObject>>(*arg0)->setField("$content", arg1);
        return Code::ReturnVoid{};
      });
    return super->methodCode(std::move(reference));
  }
};
