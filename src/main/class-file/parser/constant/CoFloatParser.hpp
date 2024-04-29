#pragma once

#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoScalarParser.hpp"

using CoFloatParser = CoScalarParser<Constant::Tag::Float, float>;
