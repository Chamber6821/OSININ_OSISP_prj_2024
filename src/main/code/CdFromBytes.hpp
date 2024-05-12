#pragma once

#include "class-file/constant/pool/ConstantPool.hpp"
#include "code/Code.hpp"
#include "code/context/Context.hpp"
#include "code/instruction-set/InstructionSet.hpp"
#include "java/object/JavaObject.hpp"
#include "p.hpp"
#include <cstdint>
#include <exception>
#include <format>
#include <stdexcept>
#include <variant>
#include <vector>

class CdFromBytes : public Code {
  std::vector<std::uint8_t> bytes;
  p<ConstantPool> pool;
  p<InstructionSet> instructionSet;

public:
  CdFromBytes(
    std::vector<std::uint8_t> bytes, p<ConstantPool> pool,
    p<InstructionSet> instructionSet
  )
      : bytes(std::move(bytes)), pool(std::move(pool)),
        instructionSet(std::move(instructionSet)) {}

  Result result(p<Context> context, p<JavaObject>) const override try {
    Result result = Next{};
    while (std::holds_alternative<Next>(result)) {
      result = instructionSet
                 ->instructionFor(
                   {bytes.begin() + context->instructionPointer()->address(),
                    bytes.end()},
                   pool
                 )
                 ->result(context);
    }
    return result;
  } catch (...) {

    std::throw_with_nested(std::runtime_error(std::format(
      "Failed while execute instruction {:X}",
      bytes.begin()[context->instructionPointer()->address()]
    )));
  }
};
