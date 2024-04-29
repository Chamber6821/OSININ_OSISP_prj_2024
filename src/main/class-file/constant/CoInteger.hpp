#pragma once

#include "class-file/constant/CoScalar.hpp"
#include "class-file/constant/Constant.hpp"
#include <cstdint>

using CoInteger = CoScalar<Constant::Tag::Integer, std::int32_t>;
