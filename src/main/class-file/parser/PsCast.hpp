#pragma once

#include "class-file/parser/Parser.hpp"
#include "p.hpp"
#include <istream>
#include <utility>

template <class F, class T>
class PsCast : public Parser<T> {
  p<Parser<F>> origin;

public:
  PsCast(p<Parser<F>> origin) : origin(std::move(origin)) {}

  bool canParse(std::istream &in) const override {
    return origin->canParse(in);
  }

  T parsed(std::istream &in) const override { return origin->parsed(in); }
};
