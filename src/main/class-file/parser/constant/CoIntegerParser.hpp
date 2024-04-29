#pragma once

#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoScalarParser.hpp"
#include <cstdint>

using CoIntegerParser = CoScalarParser<Constant::Tag::Integer, std::int32_t>;
