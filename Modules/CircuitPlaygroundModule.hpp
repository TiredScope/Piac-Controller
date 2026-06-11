#ifndef _CIRCUITPLAYGROUNDMODULE_H_
#define _CIRCUITPLAYGROUNDMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#define CIRCUITPLAYGROUND_DEFAULT_DELAY 1000

enum class CircuitPlaygroundComponent : uint8_t {
  BUTTONS,
  CAPACITIVE_TOUCH,
  ACCELEROMETER,
  TEMPERATURE,
  LIGHT,
  SOUND,

  MAX_VALUE,
};

#define CIRCUITPLAYGROUND_COMPONENT_FLAG(component) (1 << (uint8_t)(component))

const uint8_t CIRCUITPLAYGROUND_CAPACITIVE_TOUCH_PINS[8] = { 0, 1, 2, 3, 6, 9, 10, 12 };

class CircuitPlaygroundModule : public Module {
private:
  uint8_t reportingMode = 0;        // 0 = all disabled
  uint8_t capacitiveTouchPins = 0;  // 0 = all disabled
  uint32_t delays[(uint8_t)CircuitPlaygroundComponent::MAX_VALUE] = { 0 };
  unsigned long lastSent[(uint8_t)CircuitPlaygroundComponent::MAX_VALUE] = { 0 };

  bool needsToBeSent(CircuitPlaygroundComponent component);
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