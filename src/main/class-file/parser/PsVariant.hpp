#pragma once

#include "class-file/parser/Parser.hpp"
#include "p.hpp"
#include <algorithm>
#include <istream>
#include <stdexcept>
#include <vector>

template <class T>
class PsVariant : public Parser<T> {
  std::vector<p<Parser<T>>> parsers;

public:
  PsVariant(std::vector<p<Parser<T>>> parsers) : parsers(std::move(parsers)) {}

  bool canParse(std::istream &in) const override {
    for (auto &el : parsers)
      if (el->canParse(in)) return true;
    return false;
  }

  T parsed(std::istream &in) const override {
    for (auto &el : parsers)
      if (el->canParse(in)) return el->parsed(in);
    throw std::runtime_error(
      std::format("Can't parse this stream at {}", (int)in.tellg())
    );
  }
};
