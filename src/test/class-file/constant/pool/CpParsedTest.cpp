#include "class-file/constant/pool/CpParsed.hpp"
#include "class-file/constant/CoInteger.hpp"
#include "class-file/constant/Constant.hpp"
#include "tool/verifyConstant.hpp"
#include <cstdint>
#include <doctest/doctest.h>
#include <sstream>

TEST_SUITE("CpParsed") {
  TEST_CASE("should parse integer") {
    struct __attribute__((packed)) {
      std::uint8_t count1;
      std::uint8_t count2;
      std::uint8_t tag;
      std::uint8_t value1;
      std::uint8_t value2;
      std::uint8_t value3;
      std::uint8_t value4;
    } pack = {0, 2, (int)Constant::Tag::Integer, 0, 0, 0, 42};

    std::stringstream buf;
    buf.write((char *)&pack, sizeof(pack));
    CHECK(
      42 ==
      verifyConstant<CoInteger>(Constant::Tag::Integer, CpParsed(buf).at(1))
        ->value()
    );
  }
}
