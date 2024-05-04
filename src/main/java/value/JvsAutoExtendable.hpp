
#pragma once

#include "java/value/JavaValue.hpp"
#include "java/value/JavaValues.hpp"
#include "p.hpp"
#include <map>

class JvsAutoExtendable : public JavaValues {
  mutable std::map<int, p<JavaValue>> cells;

public:
  JvsAutoExtendable(std::map<int, p<JavaValue>> cells)
      : cells(std::move(cells)) {}

  JvsAutoExtendable() : JvsAutoExtendable(std::map<int, p<JavaValue>>{}) {}

  p<JavaValue> at(int index) const override { return cells[index]; }
};
