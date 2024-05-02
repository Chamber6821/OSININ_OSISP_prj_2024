#pragma once

#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/interfaces/Interfaces.hpp"
#include "tool/readInt.hpp"
#include "tool/verifyConstant.hpp"
#include <cstdint>
#include <vector>

class IeParsed : public Interfaces {
  p<ConstantPool> pool;
  std::vector<std::uint16_t> interfaces;

public:
  IeParsed(std::istream &in, p<ConstantPool> pool) : pool(std::move(pool)) {
    std::uint16_t count;
    readInt(in, count);
    interfaces.assign(count, 0);
    for (int i = 0; i < count; i++) {
      readInt(in, interfaces[i]);
    }
  }

  int count() const override { return interfaces.size(); }

  p<CoClass> at(int index) const override {
    return verifyConstant<CoClass>(
      Constant::Tag::Class,
      pool->at(interfaces.at(index))
    );
  }
};
