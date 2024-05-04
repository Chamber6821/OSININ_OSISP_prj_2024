#pragma once

#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/methods/MeParsed.hpp"
#include "class-file/methods/Method.hpp"
#include "class-file/methods/Methods.hpp"
#include "make.hpp"
#include "tool/readInt.hpp"
#include <cstdint>
#include <exception>
#include <istream>
#include <stdexcept>
#include <vector>

class MsParsed : public Methods {
  std::vector<p<Method>> methods;

public:
  MsParsed(std::istream &in, p<ConstantPool> pool) try {
    std::uint16_t count;
    readInt(in, count);
    methods.assign(count, nullptr);
    for (auto &method : methods)
      method = make<MeParsed>(in, pool);
  } catch (...) {

    std::throw_with_nested(std::runtime_error("Failed while parse methods"));
  }

  int count() const override { return methods.size(); }

  p<Method> at(int index) const override { return methods.at(index); }
};
