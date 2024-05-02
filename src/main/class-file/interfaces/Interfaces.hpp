#pragma once

#include "Interface.hpp"
#include "class-file/constant/CoClass.hpp"

class Interfaces : public Interface {
public:
  virtual int count() const = 0;
  virtual p<CoClass> at(int index) const = 0;
};
