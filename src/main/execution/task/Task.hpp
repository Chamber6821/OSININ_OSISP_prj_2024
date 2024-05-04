#pragma once

#include "Interface.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"

class Task : public Interface {
public:
  virtual p<Iterable<p<Task>>> continuation() = 0;
};
