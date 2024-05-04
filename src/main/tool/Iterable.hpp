#pragma once

#include "Interface.hpp"

template <class T>
class Iterable : public Interface {
public:
  virtual bool hasNext() const = 0;
  virtual T next() = 0;
};
