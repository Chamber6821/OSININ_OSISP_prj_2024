#include <cstdlib>
#include <doctest/doctest.h>
#include <fstream>

TEST_SUITE("it") {
    TEST_CASE("Parse resource/java/Sum") {
        std::system("javac ./resource/java/Sum.java");
        SUBCASE("Count of constants must be equal 22") {
            REQUIRE(
                make<CzFile>("./resource/java/Sum.class")
                    ->constants()
                    ->count() == 22
            );
        }
        std::system("rm ./resource/java/Sum.class");
    }
}
