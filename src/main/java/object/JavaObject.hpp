#pragma once

#include "Interface.hpp"
#include "java/class/JavaClass.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"
#include <string>

class JavaObject : public Interface {
public:
  virtual p<JavaClass> type() const = 0;
  virtual p<JavaValue> field(std::string name) const = 0;
  virtual void setField(std::string name, p<JavaValue> value) = 0;
};
