#pragma once

#include "class-file/constant/Constant.hpp"

template <Constant::Tag T>
class CoWithTag : public Constant {
public:
  static constexpr Tag TAG = T;

  Tag tag() const override { return T; }
};
