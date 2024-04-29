#pragma once

#include "class-file/constant/Constant.hpp"

template <Constant::Tag TAG>
class CoWithTag : public Constant {
public:
  Tag tag() const override { return TAG; }
};
