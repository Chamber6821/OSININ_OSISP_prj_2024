#pragma once

#include "java/class/JcsMap.hpp"
#include "java/core/Runtime.hpp"
#include "java/core/Stdout.hpp"
#include "java/lang/ObjectClass.hpp"
#include "java/lang/StringClass.hpp"

class JcsSystem : public JcsMap {
public:
  JcsSystem() {
    auto objectClass = make<ObjectClass>();
    auto stringClass = make<StringClass>(objectClass);
    auto stdoutClass = make<Stdout>(std::cout, objectClass);
    auto runtimeClass = make<Runtime>(stdoutClass->newObject(stdoutClass));
    add(objectClass->name(), objectClass);
    add(stringClass->name(), stringClass);
    add(stdoutClass->name(), stdoutClass);
    add(runtimeClass->name(), runtimeClass);
  }
};
