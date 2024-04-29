
#pragma once

#include "class-file/constant/Constant.hpp"
#include "class-file/parser/Parser.hpp"
#include "p.hpp"

template <Constant::Tag TAG, class T>
class CoParser : public Parser<p<T>> {
public:
  bool canParse(std::istream &in) const override {
    return in.peek() == (int)TAG;
  }
};
