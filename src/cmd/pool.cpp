#include "class-file/constant/pool/CpParsed.hpp"
#include "tool/readInt.hpp"
#include "tool/stringify/constant.hpp"
#include "tool/stringify/exception.hpp"
#include <cstdint>
#include <exception>
#include <format>
#include <fstream>
#include <iostream>

int main() {
  try {
    const char *classPath = "resource/java/Sum.class";
    std::ifstream fin(classPath, std::ios::binary);

    struct {
      std::uint32_t magic;
      std::uint16_t minorVersion;
      std::uint16_t majorVersion;
    } prefix;

    readInt(fin, prefix.magic);
    readInt(fin, prefix.minorVersion);
    readInt(fin, prefix.majorVersion);
    std::cout << std::format(
      "Prefix: 0x{:04X} {}.{}\n",
      prefix.magic,
      prefix.majorVersion,
      prefix.minorVersion
    );

    CpParsed pool(fin);
    for (int i = 1; i < 100; i++) {
      std::cout << std::format("#{}: {}\n", i, stringify(pool.at(i)));
    }

  } catch (const std::exception &e) {
    std::cerr << stringify(e);
  }
}
