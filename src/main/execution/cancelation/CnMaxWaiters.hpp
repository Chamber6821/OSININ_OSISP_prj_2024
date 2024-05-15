#pragma once

#include "execution/cancelation/Cancelation.hpp"
#include "p.hpp"
#include <atomic>

class CnMaxWaiters : public Cancelation {
  std::atomic<int> waiters = 0;
  int maxWaiters;
  p<Cancelation> origin;

public:
  CnMaxWaiters(int maxWaiters, p<Cancelation> origin)
      : maxWaiters(maxWaiters), origin(std::move(origin)) {}

  bool stopped() const override { return origin->stopped(); }

  void stop() override { return origin->stop(); }

  void wait() override {
    waiters++;
    if (waiters >= maxWaiters) {
      stop();
      return;
    }
    origin->wait();
    waiters--;
  }

  void wakeup() override { origin->wakeup(); }
};
