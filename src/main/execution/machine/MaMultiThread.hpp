#pragma once

#include "execution/cancelation/CnMaxWaiters.hpp"
#include "execution/cancelation/CnSimple.hpp"
#include "execution/machine/MaSingleThread.hpp"
#include "execution/machine/Machine.hpp"
#include "execution/queue/QuConcurrent.hpp"
#include "execution/queue/QuWaiting.hpp"
#include "execution/queue/Queue.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/stringify/exception.hpp"
#include <thread>
#include <utility>

class MaMultiThread : public Machine {
  int threadCount;
  p<Queue> tasks;

public:
  MaMultiThread(int threadCount, p<Queue> tasks)
      : threadCount(threadCount), tasks(std::move(tasks)) {}

  void run() override {
    auto cancelation = make<CnMaxWaiters>(threadCount, make<CnSimple>());
    auto queue = make<QuWaiting>(cancelation, make<QuConcurrent>(tasks));
    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; i++) {
      threads.emplace_back([&]() {
        try {
          MaSingleThread(queue).run();
        } catch (const std::exception &e) {
          std::cout << stringify(e);
        }
        cancelation->stop();
      });
    }
    for (auto &thread : threads)
      thread.join();
  }
};
