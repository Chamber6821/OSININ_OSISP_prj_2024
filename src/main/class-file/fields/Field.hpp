#pragma once

#include "Interface.hpp"
#include "class-file/attribute/Attributes.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include "p.hpp"

class Field : public Interface {
public:
  virtual p<CoUtf8> name() const = 0;
  virtual p<CoUtf8> descriptor() const = 0;
  virtual p<Attributes> attributes() const = 0;
};
