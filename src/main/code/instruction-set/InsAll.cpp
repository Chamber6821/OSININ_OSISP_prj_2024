#include "InsAll.hpp"
#include "java/class/JavaClasses.hpp"
#include "p.hpp"

InsAll::InsAll(p<JavaClasses>)
    : InsMappedByOpcode(std::map<std::uint8_t, p<InstructionSet>>{}) {}
