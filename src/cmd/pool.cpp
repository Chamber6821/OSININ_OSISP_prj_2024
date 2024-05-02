#include "class-file/CfParsed.hpp"
#include "make.hpp"
#include "tool/stringify/constant.hpp"
#include "tool/stringify/exception.hpp"
#include <exception>
#include <format>
#include <fstream>
#include <iostream>

int main() {
  try {
    const char *classPath = "resource/java/Sum.class";
    std::ifstream fin(classPath, std::ios::binary);
    auto classFile = make<CfParsed>(fin);

    for (int i = 1; i < 100; i++) {
      std::cout << std::format(
        "#{}: {}\n",
        i,
        stringify(classFile->constantPool()->at(i))
      );
    }

  } catch (const std::exception &e) {
    std::cerr << stringify(e);
  }
}
