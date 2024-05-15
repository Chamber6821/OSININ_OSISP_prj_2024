#pragma once

#include "Interface.hpp"

class Cancelation : public Interface {
public:
  virtual bool stopped() const = 0;
  virtual void stop() = 0;
  virtual void wait() = 0;
  virtual void wakeup() = 0;
};
