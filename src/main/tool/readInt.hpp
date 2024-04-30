#pragma once

#include "tool/reverseBytes.hpp"
#include <istream>

template <class T>
void readInt(std::istream &in, T &integer) {
  in.read((char *)&integer, sizeof(integer));
  integer = reverseBytes(integer);
}
