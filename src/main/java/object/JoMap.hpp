#pragma once

#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"
#include <map>
#include <stdexcept>
#include <string>

class JoMap : public JavaObject {
  std::map<std::string, p<JavaValue>> map;

public:
  p<JavaClass> type() const override {
    throw std::runtime_error("I am stupid box with data without type");
  }

  p<JavaValue> field(std::string name) const override { return map.at(name); }

  void setField(std::string name, p<JavaValue> value) override {
    map[name] = value;
  }
};
