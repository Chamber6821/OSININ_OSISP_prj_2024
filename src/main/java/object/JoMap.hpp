#pragma once

#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"
#include <map>
#include <string>

class JoMap : public JavaObject {
  p<JavaClass> _type;
  std::map<std::string, p<JavaValue>> map;

public:
  JoMap(p<JavaClass> type) : _type(std::move(type)) {}

  p<JavaClass> type() const override { return _type; }

  p<JavaValue> field(std::string name) const override { return map.at(name); }

  void setField(std::string name, p<JavaValue> value) override {
    map[name] = value;
  }
};
