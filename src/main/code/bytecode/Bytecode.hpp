#pragma once

#include "Interface.hpp"
#include "code/Code.hpp"
#include "p.hpp"

class Bytecode : public Interface {
public:
  virtual p<Code> instruction(int address) const = 0;
};
