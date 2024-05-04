#pragma once

#include "code/Code.hpp"
#include "code/context/Context.hpp"
#include "code/context/CxWrap.hpp"
#include "code/context/instruction-pointer/IpSimple.hpp"
#include "code/context/stack/SfUnlimited.hpp"
#include "execution/task/Task.hpp"
#include "java/value/JvsAutoExtendable.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"
#include <stack>
#include <stdexcept>
#include <utility>
#include <variant>

class JavaTask : public Task {
  std::stack<std::pair<p<Code>, p<Context>>> stack;

  struct Visitor {
    JavaTask *that;

    void operator()(Code::Next) {
      throw std::runtime_error("Unexpected code result: Next");
    }

    void operator()(Code::ReturnVoid) { that->stack.pop(); }

    void operator()(Code::ReturnValue result) {
      that->stack.pop();
      that->stack.top().second->stack()->push(std::move(result.value));
    }

    void operator()(Code::Call call) {
      that->stack.emplace(
        call.type->methodCode(call.methodName, call.methodSignature),
        make<CxWrap>(
          make<IpSimple>(),
          make<SfUnlimited>(),
          make<JvsAutoExtendable>(),
          std::move(call.arguments)
        )
      );
    }

    void operator()(Code::Throw) {
      throw std::runtime_error("Uncaught Java exception");
    }
  };

public:
  JavaTask(std::stack<std::pair<p<Code>, p<Context>>> stack)
      : stack(std::move(stack)) {}

  JavaTask(Code::Call call) { Visitor{this}(std::move(call)); }

  p<Iterable<p<Task>>> continuation() override {
    while (not stack.empty()) {
      auto [code, context] = stack.top();
      auto result = code->result(context);
      std::visit(Visitor{this}, result);
    }
    return make<Iterable<p<Task>>::Empty>();
  }
};
