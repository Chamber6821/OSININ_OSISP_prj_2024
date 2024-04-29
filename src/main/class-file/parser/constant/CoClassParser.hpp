#pragma once

#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/parser/constant/CoParser.hpp"
#include "make.hpp"
#include "p.hpp"
#include <cstdint>

class CoClassParser : public CoParser<Constant::Tag::Class, CoClass> {
  p<ConstantPool> pool;

public:
  CoClassParser(p<ConstantPool> pool) : pool(std::move(pool)) {}

  p<CoClass> parsed(std::istream &in) const override {
    in.get();
    std::uint16_t index = 0;
    in.read((char *)&index, sizeof(index));
    return make<CoClass>(pool, (int)index);
  }
};
