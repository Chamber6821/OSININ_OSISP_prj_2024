#pragma once

#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"
#include <atomic>
#include <cstdint>
#include <format>
#include <map>
#include <stdexcept>
#include <string>

class JoMap : public JavaObject {
  p<JavaClass> _type;
  std::map<std::string, p<JavaValue>> map;
  std::atomic<bool> monitor = false;

public:
  JoMap(p<JavaClass> type) : _type(std::move(type)) {}

  p<JavaClass> type() const override { return _type; }

  p<JavaValue> field(std::string name) const override {
    if (not map.contains(name))
      throw std::runtime_error(std::format(
        "Object {:X} has no attribute {}",
        (std::intptr_t)this,
        name
      ));
    return map.at(name);
  }

  void setField(std::string name, p<JavaValue> value) override {
    map[name] = value;
  }

  bool tryLock() override {
    bool locked = false;
    return monitor.compare_exchange_strong(locked, true);
  }

  void unlock() override { monitor.exchange(false); }
};
