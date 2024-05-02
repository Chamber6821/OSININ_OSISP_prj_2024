#pragma once

#include "Interface.hpp"
#include "class-file/methods/Method.hpp"

class Methods : public Interface {
public:
  virtual int count() const = 0;
  virtual p<Method> at(int index) const = 0;
};
