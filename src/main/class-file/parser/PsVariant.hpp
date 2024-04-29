#pragma once

#include "class-file/parser/Parser.hpp"
#include "p.hpp"
#include <algorithm>
#include <istream>
#include <ranges>
#include <vector>

template <class T>
class PsVariant : public Parser<T> {
  std::vector<p<Parser<T>>> parsers;

public:
  PsVariant(std::vector<p<Parser<T>>> parsers) : parsers(std::move(parsers)) {}

  bool canParse(std::istream &in) const override {
    return std::ranges::any_of(parsers | std::views::transform([&](auto p) {
                                 return p->canParse(in);
                               }));
  }

  T parse(std::istream &in) const override {
    return (parsers |
            std::views::filter([&](auto p) { return p->canParse(in); })
    )->parse(in);
  }
};
