#pragma once

#include "Interface.hpp"
#include "class-file/constant/CoNameAndType.hpp"
#include "code/Code.hpp"
#include "java/object/JavaObject.hpp"
#include "p.hpp"

class JavaClass : public Interface {
public:
  virtual p<JavaObject> newObject() const = 0;
  virtual p<Code> methodCode(p<CoNameAndType> ref) const = 0;
};
