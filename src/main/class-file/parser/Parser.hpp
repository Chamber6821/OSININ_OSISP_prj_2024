#pragma once

#include "Interface.hpp"
#include <istream>

template <class T>
class Parser : public Interface {
public:
  virtual bool canParse(std::istream &in) const = 0;
  virtual T parsed(std::istream &in) const = 0;
};
