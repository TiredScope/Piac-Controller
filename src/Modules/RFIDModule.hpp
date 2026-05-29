#include "SPI.h"
#ifndef _RFIDMODULE_H_
#define _RFIDMODULE_H_

#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>

#include "../../Module.hpp"
#include "../../Common.hpp"

class RFIDModule : public Module {
private:
  SPIClass &spi;
  MFRC522 mfrc522;
  bool cardRead;
  bool enabled;
public:
  RFIDModule(pin_t ssPin, pin_t rstPin, SPIClass &spi = SPI)
    : spi(spi), mfrc522(ssPin, rstPin){};

  virtual const char *getId() const override {
    return "rfid";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#endif