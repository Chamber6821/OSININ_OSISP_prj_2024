#pragma once

#include "Interface.hpp"
#include "class-file/constant/CoClass.hpp"
#include "java/class/JavaClass.hpp"

class JavaClasses : public Interface {
public:
  virtual bool has(p<CoClass> ref) const = 0;
  virtual p<JavaClass> type(p<CoClass> ref) const = 0;
};
