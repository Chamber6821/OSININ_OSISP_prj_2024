#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/Constant.hpp"
#include "make.hpp"
#include <doctest/doctest.h>
#include <map>
#include <string>

class FakeConstantPool : public ConstantPool {
  std::map<int, p<Constant>> map;

public:
  FakeConstantPool(std::map<int, p<Constant>> map) : map(std::move(map)) {}

  FakeConstantPool() : FakeConstantPool(std::map<int, p<Constant>>{}) {}

  p<Constant> at(int index) const override { return map.at(index); }
};

TEST_SUITE("CoClass") {
  TEST_CASE("should return the same constant") {
    auto constant = make<CoUtf8>(std::wstring());
    CHECK(
      constant ==
      make<CoClass>(
        make<FakeConstantPool>(std::map<int, p<Constant>>{{34, constant}}),
        34
      )
        ->name()
    );
  }
}
