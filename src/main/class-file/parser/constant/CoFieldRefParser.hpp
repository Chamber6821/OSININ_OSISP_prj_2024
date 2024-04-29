#pragma once

#include "class-file/constant/CoFieldRef.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoSingleRefParser.hpp"

using CoFieldRefParser = CoSingleRefParser<Constant::Tag::FieldRef, CoFieldRef>;
