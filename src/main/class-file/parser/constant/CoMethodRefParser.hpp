#pragma once

#include "class-file/constant/CoMethodRef.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoRefParser.hpp"

using CoMethodRefParser = CoRefParser<Constant::Tag::MethodRef, CoMethodRef>;
