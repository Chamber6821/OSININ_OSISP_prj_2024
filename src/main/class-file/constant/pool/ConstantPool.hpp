#pragma once

#include "Interface.hpp"
#include "class-file/constant/Constant.hpp"
#include "p.hpp"

class ConstantPool : public Interface {
public:
  virtual p<Constant> at(int index) const = 0;
};
