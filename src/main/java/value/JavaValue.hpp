#pragma once

#include "p.hpp"
#include <cstdint>
#include <stdexcept>
#include <type_traits>

class JavaObject;

class JavaValue {
public:
  virtual std::int32_t asInt() const {
    throw std::runtime_error("Value is not int");
  }

  virtual float asFloat() const {
    throw std::runtime_error("Value is not float");
  }

  virtual std::int64_t asLong() const {
    throw std::runtime_error("Value is not long");
  }

  virtual double asDouble() const {
    throw std::runtime_error("Value is not double");
  }

  virtual p<JavaObject> asObject() const {
    throw std::runtime_error("Value is not object");
  }

  template <class T>
  T as() const {
    if constexpr (std::is_same_v<T, std::int32_t>) return asInt();
    if constexpr (std::is_same_v<T, std::int64_t>) return asLong();
    if constexpr (std::is_same_v<T, float>) return asFloat();
    if constexpr (std::is_same_v<T, double>) return asDouble();
    if constexpr (std::is_same_v<T, p<JavaObject>>) return asObject();
  }
};
