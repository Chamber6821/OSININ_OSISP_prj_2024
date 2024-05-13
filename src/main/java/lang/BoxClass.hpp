#pragma once

#include "code/Code.hpp"
#include "code/MethodReference.hpp"
#include "code/context/Context.hpp"
#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "make.hpp"
#include "p.hpp"
#include <cstddef>
#include <exception>
#include <format>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

class BoxClass : public JavaClass,
                 public std::enable_shared_from_this<BoxClass> {
  std::string _name;
  std::function<p<JavaObject>(p<JavaValue>)> toString;
  p<JavaClass> super;

public:
  BoxClass(
    std::string name, std::function<p<JavaObject>(p<JavaValue>)> toString,
    p<JavaClass> super
  )
      : _name(std::move(name)), toString(std::move(toString)),
        super(std::move(super)) {}

  BoxClass(
    std::string name, std::function<std::string(p<JavaValue>)> toString,
    std::function<p<JavaObject>()> emptyString, p<JavaClass> super
  )
      : BoxClass(
          std::move(name),
          [toString, emptyString](p<JavaValue> value) {
            auto str = toString(std::move(value));
            auto string = emptyString();
            auto array = std::get<p<JavaObject>>(*string->field("$content"));
            for (std::size_t i = 0; i < str.size(); i++) {
              array->setField(
                std::format("${}", i),
                make<JavaValue>(std::int32_t(str[i]))
              );
            }
            array->setField(
              "$length",
              make<JavaValue>(std::int32_t(str.size()))
            );
            return string;
          },
          std::move(super)
        ) {}

  template <class T>
  BoxClass(
    std::string name, std::function<std::string(T)> toString,
    std::function<p<JavaObject>()> emptyString, p<JavaClass> super
  )
      : BoxClass(
          std::move(name),
          [toString](p<JavaValue> value) {
            return toString(std::get<T>(*value));
          },
          std::move(emptyString), std::move(super)
        ) {}

  BoxClass(std::string name, auto toString, p<JavaClass> super)
      : BoxClass(std::move(name), std::function(toString), std::move(super)) {}

  BoxClass(
    std::string name, auto toString, auto emptyString, p<JavaClass> super
  )
      : BoxClass(
          std::move(name), std::function(toString), std::function(emptyString),
          std::move(super)
        ) {}

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
        return Code::ReturnValue{make<JavaValue>(std::move(object))};
      });
    if (method.equal("toString", "()Ljava/lang/String;"))
      return make<Code::Wrap>([this](p<Context> context) {
        return Code::ReturnValue{make<JavaValue>(toString(
          std::get<p<JavaObject>>(*context->locals()->at(0))->field("$content")
        ))};
      });
    return super->methodCode(std::move(method));
  } catch (...) {

    std::throw_with_nested(std::runtime_error(
      std::format("Failed while search code for method from class {}", name())
    ));
  }
};
