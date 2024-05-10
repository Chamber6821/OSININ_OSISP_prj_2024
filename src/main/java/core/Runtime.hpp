#pragma once

#include "code/Code.hpp"
#include "code/MethodReference.hpp"
#include "code/context/Context.hpp"
#include "execution/task/Task.hpp"
#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"
#include <format>
#include <stdexcept>
#include <string>

class Runtime : public JavaClass {
  p<JavaObject> stdout;

public:
  Runtime(p<JavaObject> stdout) : stdout(std::move(stdout)) {}

  std::string name() const override { return "core/Runtime"; }

  p<JavaObject> newObject(p<JavaClass>) const override {
    throw std::runtime_error(
      std::format("It is forbidden to create an instance of {}", name())
    );
  }

  p<Code> methodCode(MethodReference reference) const override {
    if (reference.equal("stdout", "()Lcore/Runtime$Ostream;"))
      return make<Code::Wrap>([this](auto, auto) {
        return Code::ReturnValue{make<JavaValue>(stdout)};
      });
    if (reference.equal("suspend", "()V"))
      return make<Code::Wrap>([](p<Context> context, auto) -> Code::Result {
        if (context->instructionPointer()->address() > 0)
          return Code::ReturnVoid{};
        context->instructionPointer()->gotoAddress(1);
        return Code::ExecuteTasks{.tasks = make<Iterable<p<Task>>::Empty>()};
      });
    throw std::runtime_error(
      std::format("Class {} has no method {}", name(), reference)
    );
  }
};
