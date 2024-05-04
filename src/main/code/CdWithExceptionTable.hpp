#pragma once

#include "code/Code.hpp"
#include "code/context/Context.hpp"
#include "code/exception-table/ExceptionTable.hpp"
#include "java/object/JavaObject.hpp"
#include "p.hpp"
#include <utility>
#include <variant>

class CdWithExceptionTable : public Code {
  p<ExceptionTable> exceptionTable;
  p<Code> origin;

public:
  CdWithExceptionTable(p<ExceptionTable> exceptionTable, p<Code> origin)
      : exceptionTable(std::move(exceptionTable)), origin(std::move(origin)) {}

  Result result(p<Context> context, p<JavaObject> exception) const override {
    Result result = Throw{exception};
    while (std::holds_alternative<Throw>(result)) {
      auto exception = std::get<Throw>(result).exception;
      if (exception) {
        auto instructionPointer = context->instructionPointer();
        auto address = instructionPointer->address();
        if (not exceptionTable->canCatch(address, exception->type()))
          return result;
        instructionPointer->gotoAddress(
          exceptionTable->handlerFor(address, exception->type())
        );
      }
      result = origin->result(context, exception);
    }
    return result;
  }
};
