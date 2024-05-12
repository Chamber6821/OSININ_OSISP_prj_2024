
#pragma once

#include "java/value/JavaValue.hpp"
#include "java/value/JavaValues.hpp"
#include "p.hpp"
#include <format>
#include <map>
#include <stdexcept>

class JvsAutoExtendable : public JavaValues {
  mutable std::map<int, p<JavaValue>> cells;

public:
  JvsAutoExtendable(std::map<int, p<JavaValue>> cells)
      : cells(std::move(cells)) {}

  JvsAutoExtendable() : JvsAutoExtendable(std::map<int, p<JavaValue>>{}) {}

  p<JavaValue> at(int index) const override {
    if (not cells.contains(index))
      throw std::runtime_error(
        std::format("Cell with index {} not defined", index)
      );
    return cells.at(index);
  }

  void put(int index, p<JavaValue> value) override {
    cells[index] = std::move(value);
  }
};
