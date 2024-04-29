#pragma once

#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/parser/constant/CoParser.hpp"
#include "make.hpp"
#include "p.hpp"
#include <cstdint>

template <Constant::Tag TAG, class T>
class CoSingleRefParser : public CoParser<TAG, T> {
  p<ConstantPool> pool;

public:
  CoSingleRefParser(p<ConstantPool> pool) : pool(std::move(pool)) {}

  p<T> parsed(std::istream &in) const override {
    in.get();
    std::uint16_t index = 0;
    in.read((char *)&index, sizeof(index));
    return make<T>(pool, (int)index);
  }
};
