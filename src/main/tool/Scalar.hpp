#pragma once

#include "Interface.hpp"

template <class T>
class Scalar : public Interface {
public:
  virtual T value() const = 0;
};
