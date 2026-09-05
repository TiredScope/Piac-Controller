#ifndef _PULSESENSORMODULE_H_
#define _PULSESENSORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"
#include "PulseSensorPlayground.h"

class PulseSensorModule : public Module {
private:
  pin_t sPin;
  int threshold;
  PulseSensorPlayground pulseSensor;
public:
  PulseSensorModule(pin_t sPin, int threshold, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), sPin(sPin), threshold(threshold){};

  virtual const char *getId() const override {
    return "pulse_sensor";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "PulseSensorModule.cpp"
#endif

#endif