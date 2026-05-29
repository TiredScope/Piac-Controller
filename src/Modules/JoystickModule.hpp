#ifndef _JOYSTICKMODULE_H_
#define _JOYSTICKMODULE_H_

#include "Arduino.h"
#include "../../Module.hpp"
#include "../../Common.hpp"

#ifndef JOYSTICK_SEND_DELAY
#define JOYSTICK_SEND_DELAY 20
#endif

class JoystickModule : public Module {
private:
  pin_t xPin, yPin, swPin;
  unsigned long lastSent;
public:
  JoystickModule(pin_t xPin, pin_t yPin, pin_t swPin)
    : xPin(xPin), yPin(yPin), swPin(swPin){};

  virtual const char *getId() const override {
    return "joystick";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#endif