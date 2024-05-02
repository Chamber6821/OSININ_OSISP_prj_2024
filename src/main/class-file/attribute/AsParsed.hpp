#pragma once

#include "class-file/attribute/AtParsed.hpp"
#include "class-file/attribute/Attribute.hpp"
#include "class-file/attribute/Attributes.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "tool/readInt.hpp"
#include <cstdint>
#include <istream>

class AsParsed : public Attributes {
  p<ConstantPool> pool;
  std::vector<p<Attribute>> attributes;

public:
  AsParsed(std::istream &in, p<ConstantPool> pool) : pool(std::move(pool)) {
    std::uint16_t count;
    readInt(in, count);
    attributes.assign(count, nullptr);
    for (auto &attribute : attributes)
      attribute = make<AtParsed>(in, this->pool);
  }

  int count() const override { return attributes.size(); }

  p<Attribute> at(int index) const override { return attributes.at(index); }
};
