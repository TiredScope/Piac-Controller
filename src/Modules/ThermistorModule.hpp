#ifndef _THERMISTORMODULE_H_
#define _THERMISTORMODULE_H_

#include "Arduino.h"
#include "../../Module.hpp"
#include "../../Common.hpp"

#define THERMISTOR_SEND_DELAY 100

class ThermistorModule : public Module {
private:
  pin_t sPin;
  unsigned long lastSent;
public:
  ThermistorModule(pin_t sPin, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), sPin(sPin){};

  virtual const char *getId() const override {
    return "thermistor";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#endif