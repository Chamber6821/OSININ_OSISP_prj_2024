#pragma once

#include "class-file/constant/pool/ConstantPool.hpp"
#include "code/Code.hpp"
#include "code/instruction-set/InstructionSet.hpp"
#include "p.hpp"
#include <cstdint>
#include <format>
#include <map>
#include <span>
#include <stdexcept>
#include <utility>

class InsMappedByOpcode : public InstructionSet {
  std::map<std::uint8_t, p<InstructionSet>> map;

public:
  InsMappedByOpcode(std::map<std::uint8_t, p<InstructionSet>> map)
      : map(std::move(map)) {}

  p<Code> instructionFor(
    std::span<const std::uint8_t> bytes, p<ConstantPool> pool
  ) const override {
    if (bytes.size() < 1) throw std::runtime_error("Got zero bytes");
    auto opcode = bytes[0];
    if (not map.contains(opcode))
      throw std::runtime_error(
        std::format("Not found instruction with opcode 0x{:02X}", opcode)
      );
    return map.at(opcode)->instructionFor(bytes.subspan(1), std::move(pool));
  }
};
