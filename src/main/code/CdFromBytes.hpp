#pragma once

#include "code/Code.hpp"
#include "code/context/Context.hpp"
#include "code/instruction-set/InstructionSet.hpp"
#include "java/object/JavaObject.hpp"
#include "p.hpp"
#include <cstdint>
#include <variant>
#include <vector>

class CdFromBytes : public Code {
  std::vector<std::uint8_t> bytes;
  p<InstructionSet> instructionSet;

public:
  CdFromBytes(std::vector<std::uint8_t> bytes, p<InstructionSet> instructionSet)
      : bytes(std::move(bytes)), instructionSet(std::move(instructionSet)) {}

  Result result(p<Context> context, p<JavaObject>) const override {
    Result result = Next{};
    while (std::holds_alternative<Next>(result)) {
      result = instructionSet
                 ->instructionFor(
                   {bytes.begin() + context->instructionPointer()->address(),
                    bytes.end()}
                 )
                 ->result(context);
    }
    return result;
  }
};
