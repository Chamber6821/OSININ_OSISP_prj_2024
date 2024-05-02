#pragma once

#include "Interface.hpp"
#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/fields/Fields.hpp"
#include "class-file/interfaces/Interfaces.hpp"
#include "class-file/methods/Methods.hpp"

class ClassFile : public Interface {
public:
  virtual p<ConstantPool> constantPool() const = 0;
  virtual p<CoClass> thisClass() const = 0;
  virtual p<CoClass> superClass() const = 0;
  virtual p<Interfaces> interfaces() const = 0;
  virtual p<Fields> fields() const = 0;
  virtual p<Methods> methods() const = 0;
  virtual p<Attributes> attributes() const = 0;
};
