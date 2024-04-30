#pragma once

#include "class-file/constant/CoNameAndType.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoRefParser.hpp"

using CoNameAndTypeParser =
  CoRefParser<Constant::Tag::NameAndType, CoNameAndType>;
