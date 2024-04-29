#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/CpMutable.hpp"
#include "make.hpp"
#include <doctest/doctest.h>
#include <map>
#include <string>

TEST_SUITE("CoClass") {
  TEST_CASE("should return the same constant") {
    auto constant = make<CoUtf8>(std::wstring());
    CHECK(
      constant == make<CoClass>(
                    make<CpMutable>(std::map<int, p<Constant>>{{34, constant}}),
                    34
                  )
                    ->name()
    );
  }
}
