#pragma once

#include "code/Code.hpp"
#include "code/MethodReference.hpp"
#include "code/context/Context.hpp"
#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/object/JoMap.hpp"
#include "java/value/JavaValue.hpp"
#include "make.hpp"
#include "p.hpp"
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

class OutputStream_Default : public JavaClass {
  class Stdout : public JavaClass {
  public:
    std::string name() const override {
      return "code/OutputStream$Default$Stdout";
    }

    p<JavaObject> newObject(p<JavaClass> type) const override {
      return make<JoMap>(type);
    }

    p<Code> methodCode(MethodReference reference) const override {
      if (reference.equal("put", "(Ljava/lang/String;)V"))
        return make<Code::Wrap>([](p<Context> context, auto) {
          auto stringObject =
            std::get<p<JavaObject>>(*context->arguments()->at(1));
          auto arrayObject =
            std::get<p<JavaObject>>(*stringObject->field("$content"));
          auto length = std::get<std::int32_t>(*arrayObject->field("$length"));
          auto string = std::string(" ", length);
          for (int i = 0; i < length; i++) {
            string[i] =
              std::get<std::int32_t>(*arrayObject->field(std::format("${}", i))
              );
          }
          std::cout << string;
          return Code::ReturnVoid{};
        });
      throw std::runtime_error(
        std::format("Class {} has no method {}", name(), reference)
      );
    }
  };

  p<JavaClass> stdoutClass = make<Stdout>();
  p<JavaObject> stdoutObject = stdoutClass->newObject(stdoutClass);

public:
  std::string name() const override { return "core/OutputStream$Default"; }

  p<JavaObject> newObject(p<JavaClass>) const override {
    throw std::runtime_error(
      std::format("It is forbidden to create an instance of {}", name())
    );
  }

  p<Code> methodCode(MethodReference reference) const override {
    if (reference.equal("stdout", "()Lcore/OutputStream;"))
      return make<Code::Wrap>([this](auto, auto) {
        return Code::ReturnValue{make<JavaValue>(stdoutObject)};
      });
    throw std::runtime_error(
      std::format("Class {} has no method {}", name(), reference)
    );
  }
};
