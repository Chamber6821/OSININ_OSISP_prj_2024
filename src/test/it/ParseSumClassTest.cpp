#include <cstdlib>
#include <doctest/doctest.h>
#include <fstream>

#include "clazz/CzFile.hpp"
#include "make.hpp"
#include "tool/exec.hpp"

using doctest::Contains;
using doctest::String;

TEST_SUITE("it") {
    TEST_CASE("Java Compiler version must be equal 21") {
        REQUIRE(String(exec("javac -version").c_str()) == Contains("21."));
    }

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
