#include <cstdlib>
#include <doctest/doctest.h>
#include <fstream>

TEST_SUITE("it") {
    TEST_CASE("Parse resource/java/Sum") {
        std::system("javac ./resource/java/Sum.java");
        std::ifstream classFile(
            "./resource/java/Sum.class",
            std::ifstream::binary
        );
        const auto klass = ClassFromFile(classFile);

        SUBCASE("Count of constants must be equal 22") {
            REQUIRE(klass.constants().count() == 22);
        }
    }
}
