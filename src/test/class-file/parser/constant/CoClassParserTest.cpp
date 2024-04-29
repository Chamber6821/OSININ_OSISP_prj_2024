#include "class-file/parser/constant/CoClassParser.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include <cstdint>
#include <doctest/doctest.h>
#include <map>
#include <sstream>
#include <string>

class FakeConstantPool : public ConstantPool {
  std::map<int, p<Constant>> map;

public:
  FakeConstantPool(std::map<int, p<Constant>> map) : map(std::move(map)) {}

  FakeConstantPool() : FakeConstantPool(std::map<int, p<Constant>>{}) {}

  p<Constant> at(int index) const override { return map.at(index); }
};

TEST_SUITE("CoClassParser") {

  TEST_CASE("should return class with index 42") {
    struct __attribute__((packed)) {
      std::uint8_t tag;
      std::uint16_t index;
    } pack = {(int)Constant::Tag::Class, 42};

    std::stringstream buf;
    buf.write((char *)&pack, sizeof(pack));
    auto constant = make<CoUtf8>(std::wstring());
    CHECK(
      constant == CoClassParser(make<FakeConstantPool>(
                                  std::map<int, p<Constant>>{{42, constant}}
                                ))
                    .parsed(buf)
                    ->name()
    );
  }
}
