#pragma once

#include "class-file/constant/CoMethodRef.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoSingleRefParser.hpp"

using CoMethodRefParser =
  CoSingleRefParser<Constant::Tag::MethodRef, CoMethodRef>;
