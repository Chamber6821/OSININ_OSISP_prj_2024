#pragma once

#include "class-file/constant/CoInterfaceMethodRef.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoSingleRefParser.hpp"

using CoInterfaceMethodRefParser =
  CoSingleRefParser<Constant::Tag::InterfaceMethodRef, CoInterfaceMethodRef>;
