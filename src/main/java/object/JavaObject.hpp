#pragma once

#include "Interface.hpp"
#include "class-file/constant/CoFieldRef.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"

class JavaObject : public Interface {
public:
  virtual p<JavaValue> field(p<CoFieldRef> ref) const = 0;
  virtual void setField(p<CoFieldRef> ref, p<JavaValue> value) = 0;
};
