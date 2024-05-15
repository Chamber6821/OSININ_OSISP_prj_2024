#pragma once

#include "execution/queue/Queue.hpp"
#include "execution/task/Task.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"
#include <mutex>
#include <optional>

class QuConcurrent : public Queue {
  std::mutex mutex;
  p<Queue> origin;

public:
  QuConcurrent(p<Queue> origin) : origin(std::move(origin)) {}

  std::optional<p<Task>> pop() override {
    std::unique_lock lock(mutex);
    return origin->pop();
  }

  void push(p<Iterable<p<Task>>> tasks) override {
    std::unique_lock lock(mutex);
    origin->push(std::move(tasks));
  }
};
