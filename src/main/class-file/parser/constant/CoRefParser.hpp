#pragma once

#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/parser/constant/CoParser.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/readInt.hpp"
#include <cstdint>
#include <utility>

template <Constant::Tag TAG, class T>
class CoRefParser : public CoParser<TAG, T> {
  p<ConstantPool> pool;

public:
  CoRefParser(p<ConstantPool> pool) : pool(std::move(pool)) {}

  p<T> parsed(std::istream &in) const override {
    in.get();
    std::uint16_t classIndex = 0;
    std::uint16_t typeIndex = 0;
    readInt(in, classIndex);
    readInt(in, typeIndex);
    return make<T>(pool, (int)classIndex, (int)typeIndex);
  }
};
