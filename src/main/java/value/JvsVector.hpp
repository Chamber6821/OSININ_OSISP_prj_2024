#pragma once

#include "java/value/JavaValue.hpp"
#include "java/value/JavaValues.hpp"
#include "make.hpp"
#include "p.hpp"
#include <vector>

class JvsVector : public JavaValues {

  std::vector<p<JavaValue>> cells;

public:
  JvsVector(int capacity) : cells(capacity, make<JavaValue>()) {}

  p<JavaValue> at(int index) const override { return cells.at(index); }

  void put(int index, p<JavaValue> value) override {
    cells.at(index) = std::move(value);
  }
};
