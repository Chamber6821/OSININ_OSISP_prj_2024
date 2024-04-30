#pragma once

#include "class-file/constant/CoScalar.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/parser/constant/CoParser.hpp"
#include "make.hpp"
#include "p.hpp"

template <Constant::Tag TAG, class T>
class CoScalarParser : public CoParser<TAG, CoScalar<TAG, T>> {
public:
  p<CoScalar<TAG, T>> parsed(std::istream &in) const override {
    in.get();
    T value = 0;
    in.read((char *)&value, sizeof(value));
    return make<CoScalar<TAG, T>>(std::move(value));
  }
};
