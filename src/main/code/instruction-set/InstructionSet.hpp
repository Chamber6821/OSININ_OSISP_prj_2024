#pragma once

#include "Interface.hpp"
#include "code/Code.hpp"
#include "p.hpp"
#include <cstdint>
#include <span>

class InstructionSet : public Interface {
public:
  virtual p<Code> instructionFor(std::span<const std::uint8_t> bytes) const = 0;
};
