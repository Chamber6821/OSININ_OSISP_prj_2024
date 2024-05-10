#pragma once

#include "execution/machine/Machine.hpp"
#include "execution/queue/Queue.hpp"
#include "p.hpp"

class MaSingleThread : public Machine {
  p<Queue> queue;

public:
  MaSingleThread(p<Queue> queue) : queue(std::move(queue)) {}

  void run() override {
    while (auto task = queue->pop())
      queue->push(task.value()->continuation());
  }
};
