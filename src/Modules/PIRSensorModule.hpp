#ifndef _JOYSTICKMODULE_H_
#define _JOYSTICKMODULE_H_

#include "Arduino.h"
#include "../../Module.hpp"
#include "../../Common.hpp"

#define PIRSENSOR_SEND_DELAY 1000

class PIRSensorModule : public Module {
private:
  pin_t sPin;
  unsigned long lastSent;
public:
  PIRSensorModule(pin_t sPin)
    : sPin(sPin){};

  virtual const char *getId() const override {
    return "pir_sensor";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#endif