#pragma once

#include "execution/machine/Machine.hpp"
#include "execution/queue/Queue.hpp"
#include "p.hpp"

class MaSingleThread : public Machine {
  p<Queue> queue;

public:
  MaSingleThread(p<Queue> queue) : queue(std::move(queue)) {}

  void run() override {
    while (not queue->empty())
      queue->push(queue->pop()->continuation());
  }
};
