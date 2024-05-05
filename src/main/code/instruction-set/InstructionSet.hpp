#pragma once

#include "Interface.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "code/Code.hpp"
#include "p.hpp"
#include <cstdint>
#include <span>

class InstructionSet : public Interface {
public:
  virtual p<Code> instructionFor(
    std::span<const std::uint8_t> bytes, p<ConstantPool> pool
  ) const = 0;
};
