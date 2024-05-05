#pragma once

#include "Interface.hpp"
#include "code/MethodReference.hpp"
#include "code/context/Context.hpp"
#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "java/value/JavaValues.hpp"
#include "p.hpp"
#include <algorithm>
#include <functional>
#include <utility>
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
    MethodReference method;
    p<JavaValues> arguments;
  };

  struct Throw {
    p<JavaObject> exception;
  };

  using Result = std::variant<Next, ReturnVoid, ReturnValue, Call, Throw>;

  class Wrap;

  virtual Result result(p<Context> context, p<JavaObject> exception) const = 0;

  Result result(p<Context> context) const {
    return result(std::move(context), nullptr);
  }
};

class Code::Wrap : public Code {
  std::function<Result(p<Context>, p<JavaObject>)> implementation;

public:
  Wrap(auto implementation) : implementation(std::move(implementation)) {}

  Result result(p<Context> context, p<JavaObject> exception) const override {
    return implementation(std::move(context), std::move(exception));
  }
};
