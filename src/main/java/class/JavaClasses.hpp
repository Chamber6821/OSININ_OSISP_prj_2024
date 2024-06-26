#pragma once

#include "Interface.hpp"
#include "java/class/JavaClass.hpp"
#include "p.hpp"
#include <string>

class JavaClasses : public Interface {
public:
  virtual bool has(std::string name) const = 0;
  virtual p<JavaClass> type(std::string name) const = 0;
  virtual void add(p<JavaClass> type) = 0;
};
