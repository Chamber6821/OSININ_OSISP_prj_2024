#pragma once

#include "execution/queue/Queue.hpp"
#include "execution/task/Task.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"
#include <optional>
#include <queue>

class QuStl : public Queue {
  std::queue<p<Task>> tasks;

public:
  QuStl(std::queue<p<Task>> tasks) : tasks(std::move(tasks)) {}

  template <class... Tasks>
  QuStl(Tasks &&...tasks) : tasks(std::deque<p<Task>>{tasks...}) {}

  std::optional<p<Task>> pop() override {
    if (tasks.empty()) return std::nullopt;
    auto task = std::move(tasks.front());
    tasks.pop();
    return task;
  }

  void push(p<Iterable<p<Task>>> tasks) override {
    while (tasks->hasNext())
      this->tasks.push(tasks->next());
  }
};
