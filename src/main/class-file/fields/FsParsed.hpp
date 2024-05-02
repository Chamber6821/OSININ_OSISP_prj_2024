#pragma once

#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/fields/FiParsed.hpp"
#include "class-file/fields/Field.hpp"
#include "class-file/fields/Fields.hpp"
#include "make.hpp"
#include "tool/readInt.hpp"
#include <cstdint>
#include <istream>
#include <vector>

class FsParsed : public Fields {
  std::vector<p<Field>> fields;

public:
  FsParsed(std::istream &in, p<ConstantPool> pool) {
    std::uint16_t count;
    readInt(in, count);
    fields.assign(count, nullptr);
    for (auto &field : fields)
      field = make<FiParsed>(in, pool);
  }

  int count() const override { return fields.size(); }

  p<Field> at(int index) const override { return fields.at(index); }
};
