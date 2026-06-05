#ifndef _CIRCUITPLAYGROUNDMODULE_H_
#define _CIRCUITPLAYGROUNDMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

class CircuitPlaygroundModule : public Module {
private:
  pin_t recvPin;
public:
  CircuitPlaygroundModule(uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator){};

  virtual const char *getId() const override {
    return "circuit_playground";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "CircuitPlaygroundModule.cpp"
#endif

#endif