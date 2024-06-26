#pragma once

#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoParser.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/readInt.hpp"
#include <cstdint>
#include <string>

class CoUtf8Parser : public CoParser<Constant::Tag::Utf8, CoUtf8> {
public:
  p<CoUtf8> parsed(std::istream &in) const override {
    in.get();
    std::uint16_t length = 0;
    readInt(in, length);
    std::string value(length, ' ');
    in.read((char *)value.data(), length);
    return make<CoUtf8>(std::move(value));
  }
};
