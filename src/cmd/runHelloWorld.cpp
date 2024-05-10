#include "class-file/CfParsed.hpp"
#include "code/Code.hpp"
#include "code/instruction-set/InsAll.hpp"
#include "execution/task/JavaTask.hpp"
#include "java/class/JcFromClassFile.hpp"
#include "java/class/JcsMap.hpp"
#include "java/core/Runtime.hpp"
#include "java/core/Stdout.hpp"
#include "java/lang/ObjectClass.hpp"
#include "java/lang/StringClass.hpp"
#include "java/value/JvsAutoExtendable.hpp"
#include "make.hpp"
#include "tool/stringify/exception.hpp"
#include <exception>
#include <fstream>
#include <iostream>

int main() {
  try {
    auto objectClass = make<ObjectClass>();
    auto stringClass = make<StringClass>(objectClass);
    auto stdoutClass = make<Stdout>(std::cout, objectClass);
    auto runtimeClass = make<Runtime>(stdoutClass->newObject(stdoutClass));
    auto classes =
      make<JcsMap>(objectClass, stringClass, runtimeClass, stdoutClass);
    std::ifstream mainClass("resource/java/HelloWorld.class", std::ios::binary);
    auto task = make<JavaTask>(Code::Call{
      .type = make<JcFromClassFile>(
        make<CfParsed>(mainClass),
        objectClass,
        classes,
        make<InsAll>(classes)
      ),
      .method = {"main", "([Ljava/lang/String;)V"},
      .arguments = make<JvsAutoExtendable>()
    });
    task->continuation();
  } catch (const std::exception &e) {
    std::cout << stringify(e);
  }
}
