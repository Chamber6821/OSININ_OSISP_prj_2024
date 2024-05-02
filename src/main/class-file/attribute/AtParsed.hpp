#pragma once

#include "class-file/attribute/Attribute.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "make.hpp"
#include "tool/readInt.hpp"
#include "tool/verifyConstant.hpp"
#include <cstdint>
#include <istream>
#include <vector>

class AtParsed : public Attribute {
  p<ConstantPool> pool;
  std::uint16_t nameIndex;
  p<std::vector<std::uint8_t>> _info;

public:
  AtParsed(std::istream &in, p<ConstantPool> pool) : pool(std::move(pool)) {
    readInt(in, nameIndex);
    std::uint32_t length;
    readInt(in, length);
    _info = make<std::vector<std::uint8_t>>(length, 0);
    for (auto &byte : *_info)
      readInt(in, byte);
  }

  p<CoUtf8> name() const override {
    return verifyConstant<CoUtf8>(Constant::Tag::Utf8, pool->at(nameIndex));
  }

  p<std::vector<std::uint8_t>> info() const override { return _info; }
};
