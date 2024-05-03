#pragma once

#include "p.hpp"
#include <cstdint>
#include <variant>

class JavaObject;

using JavaValue =
  std::variant<std::int32_t, float, std::int64_t, double, p<JavaObject>>;
