#pragma once

#include "tool/Scalar.hpp"
#include <utility>

template <class T>
class MemScalar : public Scalar<T> {
  T _value;

public:
  MemScalar(T value) : _value(std::move(value)) {}

  T value() const override { return _value; }
};
