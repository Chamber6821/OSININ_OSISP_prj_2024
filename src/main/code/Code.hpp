#pragma once

#include "Interface.hpp"
#include "class-file/constant/CoMethodRef.hpp"
#include "code/context/Context.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"
#include <cstdint>
#include <variant>
#include <vector>

class Code : public Interface {
public:
  struct Next {
    std::uint64_t address;
  };

  struct ReturnVoid {};

  struct ReturnValue {
    p<JavaValue> value;
  };

  struct Call {
    p<CoMethodRef> method;
    std::vector<p<JavaValue>> arguments;
  };

  struct Throw {
    p<JavaObject> exception;
  };

  using Result = std::variant<Next, ReturnVoid, ReturnValue, Call, Throw>;

  virtual Result result(p<Context> context) = 0;
};
