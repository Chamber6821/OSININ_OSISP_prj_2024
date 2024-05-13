#pragma once

#include "java/class/JavaClass.hpp"
#include "java/class/JavaClasses.hpp"
#include "p.hpp"
#include <memory>
#include <stdexcept>

class JcsWeakWrap : public JavaClasses {
  std::weak_ptr<JavaClasses> origin;

public:
  JcsWeakWrap(p<JavaClasses> origin) : origin(origin) {}

  bool has(std::string name) const override {
    auto classes = origin.lock();
    if (not classes) throw std::runtime_error("Class list not available");
    return classes->has(std::move(name));
  }

  p<JavaClass> type(std::string name) const override {
    auto classes = origin.lock();
    if (not classes) throw std::runtime_error("Class list not available");
    return classes->type(std::move(name));
  }

  void add(p<JavaClass> type) override {
    auto classes = origin.lock();
    if (not classes) throw std::runtime_error("Class list not available");
    classes->add(std::move(type));
  }
};
