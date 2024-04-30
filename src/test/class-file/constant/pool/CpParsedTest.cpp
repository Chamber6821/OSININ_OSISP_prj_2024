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
      std::uint16_t count;
      std::uint8_t tag;
      std::int32_t value;
    } pack = {2, (int)Constant::Tag::Integer, 42};

    std::stringstream buf;
    buf.write((char *)&pack, sizeof(pack));
    CHECK(
      42 ==
      verifyConstant<CoInteger>(Constant::Tag::Integer, CpParsed(buf).at(0))
        ->value()
    );
  }
}
