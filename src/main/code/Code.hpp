#pragma once

#include "Interface.hpp"
#include "code/MethodReference.hpp"
#include "code/context/Context.hpp"
#include "execution/task/Task.hpp"
#include "java/class/JavaClass.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "java/value/JavaValues.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"
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

  struct ExecuteTasks {
    p<Iterable<p<Task>>> tasks;
  };

  using Result =
    std::variant<Next, ReturnVoid, ReturnValue, Call, Throw, ExecuteTasks>;

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

  Wrap(std::function<Result(p<Context>)> implementation)
      : Wrap([implementation = std::move(implementation)](auto context, auto) {
          return implementation(std::move(context));
        }) {}

  Result result(p<Context> context, p<JavaObject> exception) const override {
    return implementation(std::move(context), std::move(exception));
  }
};
