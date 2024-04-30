#pragma once

#include "class-file/constant/CoFieldRef.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoRefParser.hpp"

using CoFieldRefParser = CoRefParser<Constant::Tag::FieldRef, CoFieldRef>;
