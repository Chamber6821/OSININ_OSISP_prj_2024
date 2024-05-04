#pragma once

#include "code/context/instruction-pointer/InstructionPointer.hpp"
#include <cstdint>

class IpSimple : public InstructionPointer {
  std::uint32_t _address;

public:
  IpSimple(std::uint32_t address) : _address(address) {}

  IpSimple() : IpSimple(0) {}

  std::uint32_t address() const override { return _address; }

  void gotoAddress(std::uint32_t address) override { _address = address; }
};
