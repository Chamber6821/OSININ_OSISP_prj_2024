#include "java/value/JvsAutoExtendable.hpp"
#include "java/value/JavaValue.hpp"
#include "make.hpp"
#include <doctest/doctest.h>

TEST_SUITE("JvsAutoExtendable") {
  TEST_CASE("should save value with any index") {
    auto values = make<JvsAutoExtendable>();
    *values->at(42) = JavaValue{34};
    CHECK(*values->at(42) == JavaValue{34});
  }
}
