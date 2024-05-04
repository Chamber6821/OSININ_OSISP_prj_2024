#pragma once

#include "Interface.hpp"
#include "p.hpp"

class Code;
class JavaObject;

class JavaClass : public Interface {
public:
  virtual std::string name() const = 0;
  virtual p<JavaObject> newObject(p<JavaClass> type) const = 0;
  virtual p<Code> methodCode(std::string name, std::string signature) const = 0;
};
