#pragma once

#include "class-file/constant/CoInterfaceMethodRef.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoRefParser.hpp"

using CoInterfaceMethodRefParser =
  CoRefParser<Constant::Tag::InterfaceMethodRef, CoInterfaceMethodRef>;
