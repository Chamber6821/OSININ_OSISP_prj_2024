#pragma once

#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/CoNameAndType.hpp"
#include "class-file/constant/CoWithTag.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "p.hpp"

template <Constant::Tag TAG>
class CoRef : public CoWithTag<TAG> {
  p<ConstantPool> pool;
  int classIndex;
  int typeIndex;

public:
  CoRef(p<ConstantPool> pool, int classIndex, int typeIndex)
      : pool(std::move(pool)), classIndex(classIndex), typeIndex(typeIndex) {}

  auto clazz() const {
    return verifyConstant<CoClass>(Constant::Tag::Class, pool->at(classIndex));
  }

  auto type() const {
    return verifyConstant<CoNameAndType>(
      Constant::Tag::NameAndType,
      pool->at(typeIndex)
    );
  }
};
