#pragma once

#include "class-file/constant/pool/ConstantPool.hpp"
#include "code/bytecode/Bytecode.hpp"
#include "code/instruction-set/InstructionSet.hpp"
#include "p.hpp"
#include <cstdint>
#include <vector>

class BtFromBytes : public Bytecode {
  std::vector<std::uint8_t> bytes;
  p<ConstantPool> pool;
  p<InstructionSet> instructionSet;

public:
  BtFromBytes(
    std::vector<std::uint8_t> bytes, p<ConstantPool> pool,
    p<InstructionSet> instructionSet
  )
      : bytes(std::move(bytes)), pool(std::move(pool)),
        instructionSet(std::move(instructionSet)) {}

  p<Code> instruction(int address) const override {
    return instructionSet->instructionFor(
      {bytes.begin() + address, bytes.end()},
      pool
    );
  }
};
