#pragma once

#include "Interface.hpp"
#include "java/class/JavaClass.hpp"
#include "p.hpp"
#include <cstdint>

class ExceptionTable : public Interface {
public:
  virtual bool canCatch(std::uint16_t address, p<JavaClass> type) const = 0;
  virtual std::uint16_t
  handlerFor(std::uint16_t address, p<JavaClass> type) const = 0;
};
