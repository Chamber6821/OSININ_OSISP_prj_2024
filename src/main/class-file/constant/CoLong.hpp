#pragma once

#include "class-file/constant/CoScalar.hpp"
#include "class-file/constant/Constant.hpp"
#include <cstdint>

using CoLong = CoScalar<Constant::Tag::Long, std::int64_t>;
