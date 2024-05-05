#pragma once

#include "java/class/JavaClass.hpp"
#include "java/class/JavaClasses.hpp"
#include "p.hpp"
#include <concepts>
#include <exception>
#include <format>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

class JcsMap : public JavaClasses {
  std::map<std::string, p<JavaClass>> map;

public:
  JcsMap(std::map<std::string, p<JavaClass>> map) : map(std::move(map)) {}

  JcsMap() : JcsMap(std::map<std::string, p<JavaClass>>{}) {}

  template <std::convertible_to<p<JavaClass>>... Classes>
  JcsMap(Classes &&...classes)
      : JcsMap(std::map<std::string, p<JavaClass>>{{classes->name(), classes}...
        }) {}

  void add(std::string name, p<JavaClass> type) { map[name] = type; }

  bool has(std::string name) const override { return map.contains(name); }

  p<JavaClass> type(std::string name) const override try {
    return map.at(name);
  } catch (...) {

    std::throw_with_nested(
      std::runtime_error(std::format("Not found class {}", name))
    );
  }
};
