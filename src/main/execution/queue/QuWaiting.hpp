#pragma once

#include "execution/cancelation/Cancelation.hpp"
#include "execution/queue/Queue.hpp"
#include "execution/task/Task.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"
#include <optional>
#include <utility>

class QuWaiting : public Queue {
  p<Cancelation> cancelation;
  p<Queue> origin;

public:
  QuWaiting(p<Cancelation> cancelation, p<Queue> origin)
      : cancelation(std::move(cancelation)), origin(std::move(origin)) {}

  std::optional<p<Task>> pop() override {
    while (not cancelation->stopped()) {
      auto task = origin->pop();
      if (task.has_value()) return task;
      cancelation->wait();
    }
    return std::nullopt;
  }

  void push(p<Iterable<p<Task>>> tasks) override {
    if (not tasks->hasNext()) return;
    origin->push(std::move(tasks));
    cancelation->wakeup();
  }
};
