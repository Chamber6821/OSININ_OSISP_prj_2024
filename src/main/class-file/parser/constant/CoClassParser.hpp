#pragma once

#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoSingleRefParser.hpp"

using CoClassParser = CoSingleRefParser<Constant::Tag::Class, CoClass>;
