#pragma once

#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/CoWithTag.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "tool/verifyConstant.hpp"

class CoNameAndType : public CoWithTag<Constant::Tag::NameAndType> {
  p<ConstantPool> pool;
  int nameIndex;
  int typeIndex;

public:
  CoNameAndType(p<ConstantPool> pool, int nameIndex, int typeIndex)
      : pool(std::move(pool)), nameIndex(nameIndex), typeIndex(typeIndex) {}

  auto name() const {
    return verifyConstant<CoUtf8>(Constant::Tag::Utf8, pool->at(nameIndex));
  }

  auto type() const {
    return verifyConstant<CoUtf8>(Constant::Tag::Utf8, pool->at(typeIndex));
  }
};
