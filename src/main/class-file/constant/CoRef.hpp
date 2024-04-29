#pragma once

#include "class-file/constant/CoNameAndType.hpp"
#include "class-file/constant/CoWithTag.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "p.hpp"

template <Constant::Tag TAG>
class CoRef : public CoWithTag<TAG> {
  p<ConstantPool> pool;
  int index;

public:
  CoRef(p<ConstantPool> pool, int index)
      : pool(std::move(pool)), index(index) {}

  auto ref() const {
    return verifyConstant<CoNameAndType>(
      Constant::Tag::NameAndType,
      pool->at(index)
    );
  }
};
