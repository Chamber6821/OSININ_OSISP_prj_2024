#pragma once

#include "code/Code.hpp"
#include "code/MethodReference.hpp"
#include "code/context/Context.hpp"
#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "make.hpp"
#include "p.hpp"
#include <exception>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>

class BoxClass : public JavaClass,
                 public std::enable_shared_from_this<BoxClass> {
  std::string _name;
  p<JavaClass> super;

public:
  BoxClass(std::string name, p<JavaClass> super)
      : _name(std::move(name)), super(std::move(super)) {}

  std::string name() const override { return _name; }

  p<JavaObject> newObject(p<JavaClass> type) const override {
    return super->newObject(type);
  }

  p<Code> methodCode(MethodReference method) const override try {
    if (method.name == "valueOf")
      return make<Code::Wrap>([this](p<Context> context) {
        auto object =
          newObject(std::const_pointer_cast<BoxClass>(shared_from_this()));
        object->setField("$content", context->locals()->at(0));
        context->stack()->push(make<JavaValue>(std::move(object)));
        return Code::ReturnValue{make<JavaValue>(nullptr)};
      });
    return super->methodCode(std::move(method));
  } catch (...) {

    std::throw_with_nested(std::runtime_error(
      std::format("Failed while search code for method from class {}", name())
    ));
  }
};
