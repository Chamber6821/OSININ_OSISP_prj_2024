#pragma once

#include <ranges>

template <class L>
auto asRange(const L &list) {
  return std::views::iota(0, list.count()) |
         std::views::transform([&](int i) { return list.at(i); });
}
