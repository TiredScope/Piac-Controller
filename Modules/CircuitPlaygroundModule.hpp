#ifndef _CIRCUITPLAYGROUNDMODULE_H_
#define _CIRCUITPLAYGROUNDMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#define CIRCUITPLAYGROUND_DEFAULT_DELAY 1000

#define CIRCUITPLAYGROUND_BUTTONS (1 << 0)
#define CIRCUITPLAYGROUND_SLIDE_SWITCH (1 << 1)
#define CIRCUITPLAYGROUND_CAPTOUCH (1 << 2)
#define CIRCUITPLAYGROUND_ACCELEROMETER (1 << 3)
#define CIRCUITPLAYGROUND_TEMPERATURE (1 << 4)
#define CIRCUITPLAYGROUND_LIGHT (1 << 5)
#define CIRCUITPLAYGROUND_SOUND (1 << 6)

class CircuitPlaygroundModule : public Module {
private:
  uint8_t reportingMode = 0;  // 0 = all disabled
  uint32_t capTouchDelay = CIRCUITPLAYGROUND_DEFAULT_DELAY;
  uint32_t accelerometerDelay = CIRCUITPLAYGROUND_DEFAULT_DELAY;
  uint32_t temperatureDelay = CIRCUITPLAYGROUND_DEFAULT_DELAY;
  uint32_t lightDelay = CIRCUITPLAYGROUND_DEFAULT_DELAY;
  uint32_t soundDelay = CIRCUITPLAYGROUND_DEFAULT_DELAY;
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