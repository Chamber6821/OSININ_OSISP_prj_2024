#pragma once

#include "code/Code.hpp"
#include "code/context/Context.hpp"
#include "code/context/CxWrap.hpp"
#include "code/context/instruction-pointer/IpSimple.hpp"
#include "code/context/stack/SfUnlimited.hpp"
#include "execution/task/Task.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"
#include <cstdint>
#include <exception>
#include <format>
#include <memory>
#include <stack>
#include <stdexcept>
#include <utility>
#include <variant>

class JavaTask : public Task, public std::enable_shared_from_this<Task> {
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

    void operator()(Code::Call call) try {
      that->stack.emplace(
        call.type->methodCode(call.method),
        make<CxWrap>(
          make<IpSimple>(),
          make<SfUnlimited>(),
          std::move(call.arguments)
        )
      );
    } catch (...) {

      std::throw_with_nested(std::runtime_error(std::format(
        "Failed while call {} of {}",
        call.method,
        call.type->name()
      )));
    }

    void operator()(Code::Throw) {
      throw std::runtime_error("Uncaught Java exception");
    }

    void operator()(Code::ExecuteTasks) {
      throw std::runtime_error("Unexpected code result: ExecuteTasks");
    }
  };

public:
  JavaTask(std::stack<std::pair<p<Code>, p<Context>>> stack)
      : stack(std::move(stack)) {}

  JavaTask(Code::Call call) try { Visitor{this}(call); } catch (...) {

    std::throw_with_nested(std::runtime_error(
      std::format("Failed while initialize stack with call {}", call.method)
    ));
  }

  p<Iterable<p<Task>>> continuation() override {
    while (not stack.empty()) {
      auto [code, context] = stack.top();
      try {
        auto result = code->result(context);
        if (std::holds_alternative<Code::ExecuteTasks>(result))
          return make<Iterable<p<Task>>::Union>(
            std::move(std::get<Code::ExecuteTasks>(result).tasks),
            make<Iterable<p<Task>>::Single>(shared_from_this())
          );
        std::visit(Visitor{this}, result);
      } catch (...) {
        std::throw_with_nested(std::runtime_error(std::format(
          "Failed while execute code at {:X} with context {:X}",
          (std::intptr_t)code.get(),
          (std::intptr_t)context.get()
        )));
      }
    }
    return make<Iterable<p<Task>>::Empty>();
  }
};
