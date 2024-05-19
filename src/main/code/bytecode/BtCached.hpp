#pragma once

#include "code/Code.hpp"
#include "code/bytecode/Bytecode.hpp"
#include "p.hpp"
#include <vector>

class BtCached : public Bytecode {
  mutable std::vector<p<Code>> cache;
  p<Bytecode> origin;

public:
  BtCached(int bytecodeLength, p<Bytecode> origin)
      : cache(bytecodeLength, nullptr), origin(std::move(origin)) {}

  p<Code> instruction(int address) const override {
    if (not cache.at(address)) cache.at(address) = origin->instruction(address);
    return cache.at(address);
  }
};
