#pragma once

#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoScalarParser.hpp"
#include <cstdint>

using CoLongParser = CoScalarParser<Constant::Tag::Long, std::int64_t>;
