#include "InsAll.hpp"

InsAll::InsAll()
    : InsMappedByOpcode(std::map<std::uint8_t, p<InstructionSet>>{}) {}
