#pragma once

#include "Interface.hpp"
#include <cstdint>

class InstructionPointer : public Interface {
public:
  virtual std::uint32_t address() const = 0;
  virtual void gotoAddress(std::uint32_t address) = 0;
};
