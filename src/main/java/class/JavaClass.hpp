#pragma once

#include "Interface.hpp"
#include "class-file/constant/CoNameAndType.hpp"
#include "p.hpp"

class Code;
class JavaObject;

class JavaClass : public Interface {
public:
  virtual p<JavaObject> newObject(p<JavaClass> type) const = 0;
  virtual p<Code> methodCode(p<CoNameAndType> ref) const = 0;
};
