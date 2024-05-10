#pragma once

#include "class-file/ClassFile.hpp"
#include "code/CdFromBytes.hpp"
#include "code/CdWithExceptionTable.hpp"
#include "code/MethodReference.hpp"
#include "code/exception-table/EtFromBytes.hpp"
#include "code/instruction-set/InsAll.hpp"
#include "code/instruction-set/InstructionSet.hpp"
#include "java/class/JavaClass.hpp"
#include "java/class/JavaClasses.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/asRange.hpp"
#include "tool/mergeBytes.hpp"
#include "tool/valueOfConstant.hpp"
#include <exception>
#include <format>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class JcFromClassFile : public JavaClass {
  p<ClassFile> classFile;
  p<JavaClass> super;
  p<JavaClasses> classes;
  p<InstructionSet> instructionSet;

public:
  JcFromClassFile(
    p<ClassFile> classFile, p<JavaClass> super, p<JavaClasses> classes,
    p<InstructionSet> instructionSet
  )
      : classFile(std::move(classFile)), super(std::move(super)),
        classes(std::move(classes)), instructionSet(std::move(instructionSet)) {
  }

  JcFromClassFile(
    p<ClassFile> classFile, p<JavaClasses> classes,
    p<InstructionSet> instructionSet
  )
      : JcFromClassFile(
          classFile, classes->type(classFile->thisClass()->name()->value()),
          classes, std::move(instructionSet)
        ) {}

  JcFromClassFile(p<ClassFile> classFile, p<JavaClasses> classes)
      : JcFromClassFile(std::move(classFile), classes, make<InsAll>(classes)) {}

  std::string name() const override {
    return classFile->thisClass()->name()->value();
  }

  p<JavaObject> newObject(p<JavaClass> type) const override {
    auto object = super->newObject(std::move(type));
    for (auto field : asRange(*classFile->fields())) {
      for (auto attribute : asRange(*field->attributes())) {
        if (attribute->name()->value() == "ConstantValue") {
          auto bytes = attribute->info();
          object->setField(
            field->name()->value(),
            valueOfConstant(
              classFile->constantPool()->at(
                mergeBytes(bytes->at(0), bytes->at(1))
              ),
              classes
            )
          );
        }
      }
    }
    return object;
  }

  p<Code> methodCode(MethodReference reference) const override try {
    for (auto method : asRange(*classFile->methods())) {
      if (reference
            .equal(method->name()->value(), method->descriptor()->value())) {
        for (auto attribute : asRange(*method->attributes())) {
          if (attribute->name()->value() == "Code") {
            try {
              auto bytes = attribute->info();
              auto prefixEnd = bytes->begin() + 4;
              auto codeLength = mergeBytes(
                prefixEnd[0],
                prefixEnd[1],
                prefixEnd[2],
                prefixEnd[3]
              );
              auto codeBegin = prefixEnd + 4;
              auto codeEnd = codeBegin + codeLength;
              auto exceptionTableLength = mergeBytes(codeEnd[0], codeEnd[1]);
              auto exceptionTableBegin = codeEnd + 2;
              auto exceptionTableEnd =
                exceptionTableBegin + exceptionTableLength;
              return make<CdWithExceptionTable>(
                make<EtFromBytes>(
                  std::vector<std::uint8_t>(
                    exceptionTableBegin,
                    exceptionTableEnd
                  ),
                  classFile->constantPool()
                ),
                make<CdFromBytes>(
                  std::vector<std::uint8_t>(codeBegin, codeEnd),
                  classFile->constantPool(),
                  instructionSet
                )
              );
            } catch (...) {
              std::throw_with_nested(
                std::runtime_error("Failed while parse code of method")
              );
            }
          }
        }
        throw std::runtime_error(
          std::format("Method {} have not body", reference)
        );
      }
    }
    return super->methodCode(std::move(reference));
  } catch (...) {

    std::throw_with_nested(std::runtime_error(std::format(
      "Failed while get method {} in class {}",
      reference,
      classFile->thisClass()->name()->value()
    )));
  }
};
