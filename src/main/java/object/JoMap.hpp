#pragma once

#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"
#include <map>
#include <string>

class JoMap : public JavaObject {
  std::map<std::string, p<JavaValue>> map;

public:
  p<JavaValue> field(std::string name) const override { return map.at(name); }

  void setField(std::string name, p<JavaValue> value) override {
    map[name] = value;
  }
};
