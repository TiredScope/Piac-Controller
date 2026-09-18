#ifndef _IRRECEIVERMODULE_H_
#define _IRRECEIVERMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

class IRReceiverModule : public Module {
private:
  pin_t recvPin;
public:
  IRReceiverModule(pin_t recvPin, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), recvPin(recvPin){};

  virtual const char *getId() const override {
    return "ir_receiver";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#ifdef INCLUDE_IMPLEMENTATION
#include "IRReceiverModule.cpp"
#endif

#endif