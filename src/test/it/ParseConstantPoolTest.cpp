#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/CpParsed.hpp"
#include "tool/verifyConstant.hpp"
#include <cstdlib>
#include <doctest/doctest.h>
#include <fstream>

TEST_SUITE("it") {
  TEST_CASE("constant #4 should be equal to 'java/lang/Object'") {
    std::system("javac resource/java/Sum.java");
    std::ifstream file("resource/java/Sum.class", std::ios::binary);
    file.seekg(8, std::ios::beg);
    CHECK(
      "java/lang/Object" ==
      verifyConstant<CoUtf8>(Constant::Tag::Utf8, CpParsed(file).at(4))->value()
    );
  }
}
