#include "class-file/parser/constant/CoClassParser.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/CpMutable.hpp"
#include <cstdint>
#include <doctest/doctest.h>
#include <map>
#include <sstream>
#include <string>

TEST_SUITE("CoClassParser") {

  TEST_CASE("should return class with index 42") {
    struct __attribute__((packed)) {
      std::uint8_t tag;
      std::uint8_t index1;
      std::uint8_t index2;
    } pack = {(int)Constant::Tag::Class, 0, 42};

    std::stringstream buf;
    buf.write((char *)&pack, sizeof(pack));
    auto constant = make<CoUtf8>(std::string());
    CHECK(
      constant ==
      CoClassParser(make<CpMutable>(std::map<int, p<Constant>>{{42, constant}}))
        .parsed(buf)
        ->name()
    );
  }
}
