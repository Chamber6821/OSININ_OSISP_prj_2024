#pragma once

#include "Interface.hpp"

class Machine : public Interface {
public:
  virtual void run() = 0;
};
