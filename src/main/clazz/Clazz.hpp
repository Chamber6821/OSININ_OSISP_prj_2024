#pragma once

#include "Interface.hpp"
#include "constants/Constants.hpp"
#include "p.hpp"

class Clazz : public Interface {
  public:
    virtual p<Constants> constants() = 0;
};