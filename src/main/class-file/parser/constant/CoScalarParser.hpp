#pragma once

#include "class-file/constant/CoInteger.hpp"
#include "class-file/constant/CoScalar.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoParser.hpp"
#include "make.hpp"
#include "p.hpp"

template <Constant::Tag TAG, class T>
class CoScalarParser : public CoParser<TAG, CoScalar<TAG, T>> {
public:
  p<T> parsed(std::istream &in) const override {
    in.get();
    T value = 0;
    in.read((char *)&value, sizeof(value));
    return make<CoInteger>(std::move(value));
  }
};
