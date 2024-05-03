#pragma once

#include "code/Code.hpp"
#include "code/instruction-set/InstructionSet.hpp"
#include "p.hpp"
#include <cstdint>
#include <functional>
#include <span>

class InsWrap : public InstructionSet {
  std::function<p<Code>(std::span<const std::uint8_t>)> implementation;

public:
  InsWrap(auto implementation) : implementation(std::move(implementation)) {}

  p<Code> instructionFor(std::span<const std::uint8_t> bytes) const override {
    return implementation(std::move(bytes));
  }
};
