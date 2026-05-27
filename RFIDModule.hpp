#ifndef _RFIDMODULE_H_
#define _RFIDMODULE_H_

#include "Arduino.h"
#include "Module.hpp"
#include "Common.hpp"

#include <MFRC522.h>

class RFIDModule : public Module {
private:
  MFRC522 mfrc522;
  bool cardRead;
  bool enabled;
public:
  RFIDModule(pin_t ssPin, pin_t rstPin)
    : mfrc522(ssPin, rstPin){};

  virtual const char *getId() const override {
    return "rfid";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#endif