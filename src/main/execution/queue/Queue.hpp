#pragma once

#include "Interface.hpp"
#include "execution/task/Task.hpp"
#include "p.hpp"
#include "tool/Iterable.hpp"

class Queue : public Interface {
public:
  virtual p<Task> pop() = 0;
  virtual void push(p<Iterable<p<Task>>> tasks) = 0;
  virtual bool empty() const = 0;
};
