#pragma once

#include "tool/ostream_formatter.hpp"
#include <format>
#include <iosfwd>
#include <ostream>
#include <string>

struct MethodReference {
  std::string name;
  std::string signature;

  constexpr bool operator==(const MethodReference &other) const = default;

  constexpr bool equal(std::string name, std::string signature) const {
    return *this == MethodReference{name, signature};
  }

  friend std::ostream &
  operator<<(std::ostream &out, const MethodReference &that) {
    return out << that.name << ":" << that.signature;
  }
};

template <>
struct std::formatter<MethodReference> : ostream_formatter {};
