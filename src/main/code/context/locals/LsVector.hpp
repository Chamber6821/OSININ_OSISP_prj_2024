#pragma once

#include "code/context/locals/Locals.hpp"
#include "java/value/JavaValue.hpp"
#include "make.hpp"
#include "p.hpp"
#include <vector>

class LsVector : public Locals {
  std::vector<p<JavaValue>> cells;

public:
  LsVector(int capacity) : cells(capacity, nullptr) {
    for (auto &cell : cells)
      cell = make<JavaValue>();
  }

  p<JavaValue> at(int index) const override { return cells.at(index); }
};
