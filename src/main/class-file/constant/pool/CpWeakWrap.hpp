#pragma once

#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "p.hpp"
#include <memory>
#include <stdexcept>

class CpWeakWrap : public ConstantPool {
  std::weak_ptr<ConstantPool> origin;

public:
  CpWeakWrap(p<ConstantPool> origin) : origin(origin) {}

  p<Constant> at(int index) const override {
    auto pool = origin.lock();
    if (not pool) throw std::runtime_error("Constant pool not available");
    return pool->at(index);
  }
};
