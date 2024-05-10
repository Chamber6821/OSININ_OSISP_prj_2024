#include "class-file/CfParsed.hpp"
#include "class-file/ClassFile.hpp"
#include "execution/machine/MaSingleThread.hpp"
#include "execution/queue/QuStl.hpp"
#include "execution/task/JavaTask.hpp"
#include "java/class/JcsFromClassFiles.hpp"
#include "java/class/JcsSystem.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/stringify/exception.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

int main(int argc, char **argv) {
  try {
    auto arguments = std::vector<std::string_view>(argv, argv + argc);
    if (arguments.size() < 2)
      throw std::runtime_error("At least one class file required, but got 0");

    auto filenames = std::span(arguments.begin() + 1, arguments.end());
    auto files = std::vector<std::ifstream>();
    for (auto filename : filenames) {
      files.emplace_back(std::string(filename), std::ios::binary);
    }
    auto parsedClasses = std::vector<p<ClassFile>>();
    for (auto &file : files) {
      parsedClasses.emplace_back(make<CfParsed>(file));
    }

    make<MaSingleThread>(
      make<QuStl>(make<JavaTask>(Code::Call{
        .type = make<JcsFromClassFiles>(
                  make<JcsSystem>(),
                  std::set(parsedClasses.begin(), parsedClasses.end())
        )
                  ->type(parsedClasses.front()->thisClass()->name()->value()),
        .method = {"main", "([Ljava/lang/String;)V"},
        .arguments = make<JvsAutoExtendable>()
      }))
    )
      ->run();
  } catch (const std::exception &e) {
    std::cout << stringify(e);
  }
}
