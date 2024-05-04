#pragma once

#include "java/class/JavaClass.hpp"
#include "java/class/JavaClasses.hpp"
#include "p.hpp"
#include <map>
#include <string>
#include <utility>

class JcsMap : public JavaClasses {
  std::map<std::string, p<JavaClass>> map;

public:
  JcsMap(std::map<std::string, p<JavaClass>> map) : map(std::move(map)) {}

  JcsMap() : JcsMap(std::map<std::string, p<JavaClass>>{}) {}

  void add(std::string name, p<JavaClass> type) { map[name] = type; }

  bool has(std::string name) const override { return map.contains(name); }

  p<JavaClass> type(std::string name) const override { return map.at(name); }
};
