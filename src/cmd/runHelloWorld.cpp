#include "class-file/CfParsed.hpp"
#include "code/Code.hpp"
#include "code/instruction-set/InsAll.hpp"
#include "execution/machine/MaSingleThread.hpp"
#include "execution/queue/QuStl.hpp"
#include "execution/task/JavaTask.hpp"
#include "java/class/JcFromClassFile.hpp"
#include "java/class/JcsSystem.hpp"
#include "java/value/JvsAutoExtendable.hpp"
#include "make.hpp"
#include "tool/stringify/exception.hpp"
#include <exception>
#include <fstream>
#include <iostream>

int main() {
  try {
    auto classes = make<JcsSystem>();
    std::ifstream mainClass("resource/java/HelloWorld.class", std::ios::binary);
    make<MaSingleThread>(make<QuStl>(make<JavaTask>(Code::Call{
                           .type = make<JcFromClassFile>(
                             make<CfParsed>(mainClass),
                             classes->type("java/lang/Object"),
                             classes,
                             make<InsAll>(classes)
                           ),
                           .method = {"main", "([Ljava/lang/String;)V"},
                           .arguments = make<JvsAutoExtendable>()
                         })))
      ->run();
  } catch (const std::exception &e) {
    std::cout << stringify(e);
  }
}
