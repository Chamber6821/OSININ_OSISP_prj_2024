#pragma once

#include "java/value/JavaValues.hpp"
#include "make.hpp"
#include <vector>

class JvsVector : public JavaValues {

  std::vector<p<JavaValue>> cells;

public:
  JvsVector(int capacity) : cells(capacity, nullptr) {
    for (auto &cell : cells)
      cell = make<JavaValue>();
  }

  p<JavaValue> at(int index) const override { return cells.at(index); }
};
