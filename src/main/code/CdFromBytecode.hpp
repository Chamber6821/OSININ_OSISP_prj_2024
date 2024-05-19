#pragma once

#include "code/Code.hpp"
#include "code/bytecode/Bytecode.hpp"
#include "code/context/Context.hpp"
#include "java/object/JavaObject.hpp"
#include "p.hpp"
#include <variant>

class CdFromBytecode : public Code {
  p<Bytecode> bytecode;

public:
  CdFromBytecode(p<Bytecode> bytecode) : bytecode(std::move(bytecode)) {}

  Result result(p<Context> context, p<JavaObject>) const override try {
    Result result = Next{};
    while (std::holds_alternative<Next>(result)) {
      result = bytecode->instruction(context->instructionPointer()->address())
                 ->result(context);
    }
    return result;
  } catch (...) {

    std::throw_with_nested(std::runtime_error(std::format(
      "Failed at address {}",
      context->instructionPointer()->address()
    )));
  }
};
