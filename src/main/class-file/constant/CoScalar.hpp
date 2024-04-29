#pragma once

#include "class-file/constant/CoWithTag.hpp"
#include "class-file/constant/Constant.hpp"
#include "tool/MemScalar.hpp"

template <Constant::Tag TAG, class T>
class CoScalar : public CoWithTag<TAG>, public MemScalar<T> {
public:
  using MemScalar<T>::MemScalar;
};
