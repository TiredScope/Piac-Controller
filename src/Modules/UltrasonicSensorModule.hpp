#ifndef _PULSESENSORMODULE_H_
#define _PULSESENSORMODULE_H_

#include "Arduino.h"
#include "../../Module.hpp"
#include "../../Common.hpp"
#include "../HC-SR04/SR04.h"

#define ULTRASONICSENSOR_SEND_DELAY 100

class UltrasonicSensorModule : public Module {
private:
  SR04 sr04;
  unsigned long lastSent;
public:
  UltrasonicSensorModule(pin_t echoPin, pin_t trigPin)
    : sr04(echoPin, trigPin){};

  virtual const char *getId() const override {
    return "ultrasonic_sensor";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#endif