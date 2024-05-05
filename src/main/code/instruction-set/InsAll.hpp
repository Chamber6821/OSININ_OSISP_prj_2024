#pragma once

#include "code/instruction-set/InsMappedByOpcode.hpp"
#include "java/class/JavaClasses.hpp"
#include "p.hpp"

class InsAll : public InsMappedByOpcode {
public:
  InsAll(p<JavaClasses> classes);
};
