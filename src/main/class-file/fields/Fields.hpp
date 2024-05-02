#pragma once

#include "Interface.hpp"
#include "class-file/fields/Field.hpp"

class Fields : public Interface {
public:
  virtual int count() const = 0;
  virtual p<Field> at(int index) const = 0;
};
