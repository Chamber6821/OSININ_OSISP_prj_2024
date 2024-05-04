#pragma once

#include "class-file/constant/Constant.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"

p<JavaValue> valueOfConstant(p<Constant> constant);
