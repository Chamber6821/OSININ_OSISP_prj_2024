#pragma once

#include "java/class/JcsMap.hpp"
#include "java/core/Runtime.hpp"
#include "java/core/Stdout.hpp"
#include "java/lang/BoxClass.hpp"
#include "java/lang/ObjectClass.hpp"
#include "java/lang/StringClass.hpp"
#include "java/value/JvInt.hpp"
#include "java/value/JvObject.hpp"
#include "make.hpp"
#include <cstdint>
#include <string>

class JcsSystem : public JcsMap {
public:
  JcsSystem() {
    auto objectClass = make<ObjectClass>();
    auto stringClass = make<StringClass>(objectClass);
    auto stdoutClass = make<Stdout>(std::cout, objectClass);
    auto runtimeClass =
      make<Runtime>(stdoutClass->newObject(stdoutClass), objectClass);
    auto integerClass = make<BoxClass>(
      "java/lang/Integer",
      [](std::int32_t x) { return std::to_string(x); },
      [=]() {
        auto array = objectClass->newObject(objectClass);
        array->setField("$length", make<JvInt>(0));
        auto string = stringClass->newObject(stringClass);
        string->setField("$content", make<JvObject>(std::move(array)));
        return string;
      },
      objectClass
    );
    add(objectClass);
    add(stringClass);
    add(stdoutClass);
    add(runtimeClass);
    add(integerClass);
  }
};
