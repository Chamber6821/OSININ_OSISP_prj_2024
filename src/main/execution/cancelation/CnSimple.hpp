#pragma once

#include "execution/cancelation/Cancelation.hpp"
#include <atomic>
#include <condition_variable>
#include <mutex>

class CnSimple : public Cancelation {
  std::mutex mutex;
  std::condition_variable cv;
  std::atomic<bool> _stopped = false;

public:
  bool stopped() const override { return _stopped; }

  void stop() override {
    _stopped = true;
    wakeup();
  }

  void wait() override {
    std::unique_lock lock(mutex);
    cv.wait(lock);
  }

  void wakeup() override { cv.notify_all(); }
};
