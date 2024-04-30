#pragma once

#include <cstddef>
#include <utility>

template <class T>
T reverseBytes(T value) {
  char *bytes = (char *)&value;
  for (std::size_t i = 0; i < sizeof(T) / 2; i++)
    std::swap(bytes[i], bytes[sizeof(T) - i - 1]);
  return value;
}
