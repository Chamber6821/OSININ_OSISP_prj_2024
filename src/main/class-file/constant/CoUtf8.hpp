#pragma once

#include "class-file/constant/CoScalar.hpp"
#include <string>

using CoUtf8 = CoScalar<Constant::Tag::Utf8, std::wstring>;
