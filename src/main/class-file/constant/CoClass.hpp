#pragma once

#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/CoWithTag.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "tool/verifyConstant.hpp"

class CoClass : public CoWithTag<Constant::Tag::Class> {
  p<ConstantPool> pool;
  int index;

public:
  CoClass(p<ConstantPool> pool, int index)
      : pool(std::move(pool)), index(index) {}

  auto name() const {
    return verifyConstant<CoUtf8>(Constant::Tag::Utf8, pool->at(index));
  }
};
