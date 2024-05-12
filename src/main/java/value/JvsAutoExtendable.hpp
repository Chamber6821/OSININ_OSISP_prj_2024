
#pragma once

#include "java/value/JavaValue.hpp"
#include "java/value/JavaValues.hpp"
#include "make.hpp"
#include "p.hpp"
#include <map>

class JvsAutoExtendable : public JavaValues {
  mutable std::map<int, p<JavaValue>> cells;

public:
  JvsAutoExtendable(std::map<int, p<JavaValue>> cells)
      : cells(std::move(cells)) {}

  JvsAutoExtendable() : JvsAutoExtendable(std::map<int, p<JavaValue>>{}) {}

  p<JavaValue> at(int index) const override {
    if (not cells.contains(index)) cells[index] = make<JavaValue>();
    return cells.at(index);
  }

  void put(int index, p<JavaValue> value) override {
    cells[index] = std::move(value);
  }
};
