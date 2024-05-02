#pragma once

#include "Interface.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include "p.hpp"
#include <cstdint>
#include <vector>

class Attribute : public Interface {
public:
  virtual p<CoUtf8> name() const = 0;
  virtual p<std::vector<std::uint8_t>> info() const = 0;
};
