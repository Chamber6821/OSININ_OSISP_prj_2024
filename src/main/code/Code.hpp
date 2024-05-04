#pragma once

#include "Interface.hpp"
#include "code/context/Context.hpp"
#include "java/class/JavaClass.hpp"
#include "java/value/JavaValue.hpp"
#include "java/value/JavaValues.hpp"
#include "p.hpp"
#include <variant>

class Code : public Interface {
public:
  struct Next {};

  struct ReturnVoid {};

  struct ReturnValue {
    p<JavaValue> value;
  };

  struct Call {
    p<JavaClass> type;
    std::string methodName;
    std::string methodSignature;
    p<JavaValues> arguments;
  };

  struct Throw {
    p<JavaObject> exception;
  };

  using Result = std::variant<Next, ReturnVoid, ReturnValue, Call, Throw>;

  virtual Result result(p<Context> context, p<JavaObject> exception) const = 0;

  Result result(p<Context> context) const {
    return result(std::move(context), nullptr);
  }
};
