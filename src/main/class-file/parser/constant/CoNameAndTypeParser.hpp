#pragma once

#include "class-file/constant/CoNameAndType.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/parser/constant/CoParser.hpp"
#include "make.hpp"
#include "p.hpp"
#include <cstdint>

class CoNameAndTypeParser
    : public CoParser<Constant::Tag::NameAndType, CoNameAndType> {
  p<ConstantPool> pool;

public:
  CoNameAndTypeParser(p<ConstantPool> pool) : pool(std::move(pool)) {}

  p<CoNameAndType> parsed(std::istream &in) const override {
    in.get();
    std::uint16_t nameIndex;
    std::uint16_t typeIndex;
    in.read((char *)&nameIndex, sizeof(nameIndex));
    in.read((char *)&typeIndex, sizeof(typeIndex));
    return make<CoNameAndType>(pool, nameIndex, typeIndex);
  }
};
