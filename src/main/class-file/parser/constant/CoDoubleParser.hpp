#pragma once

#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoScalarParser.hpp"

using CoDoubleParser = CoScalarParser<Constant::Tag::Double, double>;
