#pragma once

#include "Interface.hpp"
#include "class-file/attribute/Attribute.hpp"

class Attributes : public Interface {
public:
  virtual int count() const = 0;
  virtual p<Attribute> at(int index) const = 0;
};
