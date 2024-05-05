#pragma once

#include "Interface.hpp"
#include "code/MethodReference.hpp"
#include "p.hpp"

class Code;
class JavaObject;

class JavaClass : public Interface {
public:
  virtual std::string name() const = 0;
  virtual p<JavaObject> newObject(p<JavaClass> type) const = 0;
  virtual p<Code> methodCode(MethodReference method) const = 0;
};
