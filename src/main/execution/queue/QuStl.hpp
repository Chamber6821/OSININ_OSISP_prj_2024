#pragma once

#include "execution/queue/Queue.hpp"
#include "execution/task/Task.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"
#include <queue>

class QuStl : public Queue {
  std::queue<p<Task>> tasks;

public:
  QuStl(std::queue<p<Task>> tasks) : tasks(std::move(tasks)) {}

  template <class... Tasks>
  QuStl(Tasks &&...tasks) : tasks(std::deque{tasks...}) {}

  p<Task> pop() override {
    auto task = std::move(tasks.front());
    tasks.pop();
    return task;
  }

  void push(p<Iterable<p<Task>>> tasks) override {
    while (tasks->hasNext())
      this->tasks.push(tasks->next());
  }
};
