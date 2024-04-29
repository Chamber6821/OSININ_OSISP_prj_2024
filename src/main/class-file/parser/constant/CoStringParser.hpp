#pragma once

#include "class-file/constant/CoString.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoSingleRefParser.hpp"

using CoStringParser = CoSingleRefParser<Constant::Tag::String, CoString>;
