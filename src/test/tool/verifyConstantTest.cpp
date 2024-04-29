#include "tool/verifyConstant.hpp"
#include "class-file/constant/CoInteger.hpp"
#include "class-file/constant/Constant.hpp"
#include "make.hpp"

#include <doctest/doctest.h>

TEST_SUITE("verifyConstant") {
  TEST_CASE("should return thesame pointer") {
    auto constant = make<CoInteger>(42);
    CHECK(
      constant == verifyConstant<CoInteger>(Constant::Tag::Integer, constant)
    );
  }
}
