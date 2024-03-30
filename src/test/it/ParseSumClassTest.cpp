#include <cstdlib>
#include <doctest/doctest.h>
#include <fstream>

TEST_SUITE("it") {
    TEST_CASE("Print structure") {
        std::system("javac ./resource/java/Sum.java");
        std::ifstream classFile(
            "./resource/java/Sum.class",
            std::ifstream::binary
        );
    }
}
