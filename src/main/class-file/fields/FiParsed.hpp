#pragma once

#include "class-file/attribute/AsParsed.hpp"
#include "class-file/attribute/Attributes.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/fields/Field.hpp"
#include "p.hpp"
#include "tool/readInt.hpp"
#include "tool/verifyConstant.hpp"
#include <cstdint>
#include <istream>

class FiParsed : public Field {
  p<ConstantPool> pool;
  std::uint16_t nameIndex;
  std::uint16_t descriptorIndex;
  p<Attributes> _attributes;

public:
  FiParsed(std::istream &in, p<ConstantPool> pool) : pool(std::move(pool)) {
    in.seekg(2, std::ios::cur);
    readInt(in, nameIndex);
    readInt(in, descriptorIndex);
    _attributes = make<AsParsed>(in, this->pool);
  }

  p<CoUtf8> name() const override {
    return verifyConstant<CoUtf8>(Constant::Tag::Utf8, pool->at(nameIndex));
  }

  p<CoUtf8> descriptor() const override {
    return verifyConstant<CoUtf8>(
      Constant::Tag::Utf8,
      pool->at(descriptorIndex)
    );
  }

  p<Attributes> attributes() const override { return _attributes; }
};
