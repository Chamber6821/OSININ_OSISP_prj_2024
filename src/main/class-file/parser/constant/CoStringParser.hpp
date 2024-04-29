#pragma once

#include "class-file/constant/CoString.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/parser/constant/CoParser.hpp"
#include "make.hpp"
#include "p.hpp"
#include <cstdint>

class CoStringParser : public CoParser<Constant::Tag::String, CoString> {
  p<ConstantPool> pool;

public:
  CoStringParser(p<ConstantPool> pool) : pool(std::move(pool)) {}

  p<CoString> parsed(std::istream &in) const override {
    in.get();
    std::uint16_t index = 0;
    in.read((char *)&index, sizeof(index));
    return make<CoString>(pool, (int)index);
  }
};
