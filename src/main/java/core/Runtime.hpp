#pragma once

#include "code/Code.hpp"
#include "code/MethodReference.hpp"
#include "code/context/Context.hpp"
#include "execution/task/JavaTask.hpp"
#include "execution/task/Task.hpp"
#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "java/value/JvInt.hpp"
#include "java/value/JvObject.hpp"
#include "java/value/JvsAutoExtendable.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"
#include <format>
#include <map>
#include <regex>
#include <stdexcept>
#include <string>
#include <utility>

class Runtime : public JavaClass {
  p<JavaObject> stdout;
  p<JavaClass> objectClass;

public:
  Runtime(p<JavaObject> stdout, p<JavaClass> objectClass)
      : stdout(std::move(stdout)), objectClass(std::move(objectClass)) {}

  std::string name() const override { return "core/Runtime"; }

  p<JavaObject> newObject(p<JavaClass>) const override {
    throw std::runtime_error(
      std::format("It is forbidden to create an instance of {}", name())
    );
  }

  p<Code> methodCode(MethodReference reference) const override {
    if (reference.equal("stdout", "()Lcore/Runtime$Ostream;"))
      return make<Code::Wrap>([this](auto, auto) {
        return Code::ReturnValue{make<JvObject>(stdout)};
      });
    if (reference.equal("suspend", "()V"))
      return make<Code::Wrap>([](p<Context> context, auto) -> Code::Result {
        if (context->instructionPointer()->address() > 0)
          return Code::ReturnVoid{};
        context->instructionPointer()->gotoAddress(1);
        return Code::ExecuteTasks{.tasks = make<Iterable<p<Task>>::Empty>()};
      });
    if (reference.equal("launch", "(Ljava/lang/Runnable;)V"))
      return make<Code::Wrap>([](p<Context> context, auto) -> Code::Result {
        if (context->instructionPointer()->address() > 0)
          return Code::ReturnVoid{};
        context->instructionPointer()->gotoAddress(1);
        auto objectValue = context->locals()->at(0);
        auto object = objectValue->asObject();
        return Code::ExecuteTasks{
          .tasks = make<Iterable<p<Task>>::Single>(make<JavaTask>(Code::Call{
            .type = object->type(),
            .method = {.name = "run", .signature = "()V"},
            .arguments = make<JvsAutoExtendable>(
              std::map<int, p<JavaValue>>{{0, std::move(objectValue)}}
            )
          }))
        };
      });
    if (reference.name == "join" and
        std::regex_match(
          reference.signature,
          std::regex(R"(\(\[{2}([BCDFIJSZ]|L[^;]+;)\)\[([BCDFIJSZ]|L[^;]+;))")
        ))
      return make<Code::Wrap>([this](p<Context> context) {
        auto object = objectClass->newObject(objectClass);
        auto objectLength = 0;
        auto array = context->locals()->at(0)->asObject();
        auto arrayLength = array->field("$length")->asInt();
        for (int i = 0; i < arrayLength; i++) {
          auto string = array->field(std::format("${}", i))->asObject();
          auto stringLength = string->field("$length")->asInt();
          for (int j = 0; j < stringLength; j++) {
            object->setField(
              std::format("${}", objectLength),
              string->field(std::format("${}", j))
            );
            objectLength++;
          }
        }
        object->setField("$length", make<JvInt>(objectLength));
        return Code::ReturnValue{make<JvObject>(std::move(object))};
      });
    throw std::runtime_error(
      std::format("Class {} has no method {}", name(), reference)
    );
  }
};
