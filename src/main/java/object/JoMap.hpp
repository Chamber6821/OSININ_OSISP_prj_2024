#pragma once

#include "class-file/constant/CoFieldRef.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"
#include <map>

class JoMap : public JavaObject {
  std::map<p<CoFieldRef>, p<JavaValue>> map;

public:
  p<JavaValue> field(p<CoFieldRef> ref) const override { return map.at(ref); }

  void setField(p<CoFieldRef> ref, p<JavaValue> value) override {
    map[ref] = value;
  }
};
