#ifndef _THERMISTORMODULE_H_
#define _THERMISTORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#include <MPU6050.h>

#define MPU6050_DEFAULT_REPORTING_DELAY 1000

class MPU6050Module : public Module {
private:
  MPU6050 mpu6050;
  uint32_t reportingDelay;
  unsigned long lastSent;
public:
  MPU6050Module(uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), reportingDelay(MPU6050_DEFAULT_REPORTING_DELAY){};

  virtual const char *getId() const override {
    return "mpu6050";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "MPU6050Module.cpp"
#endif

#endif