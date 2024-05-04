#pragma once

#include "code/instruction-set/InsMappedByOpcode.hpp"
#include "code/instruction-set/InstructionSet.hpp"
#include "p.hpp"
#include <map>

class InsAll : public InsMappedByOpcode {
public:
  InsAll() : InsMappedByOpcode(std::map<std::uint8_t, p<InstructionSet>>{}) {}
};
