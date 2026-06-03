#ifndef _NUNCHUKMODULE_H_
#define _NUNCHUKMODULE_H_

#include "Arduino.h"
#include "../../Module.hpp"
#include "../../Common.hpp"
#include <WiiChuck.h>
#include <Nunchuck.h>

#define NUNCHUK_SEND_DELAY 20

class NunchukModule : public Module {
private:
  Accessory nunchuk;
  unsigned long lastSent;
public:
  NunchukModule(TwoWire& wire = Wire, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), nunchuk(wire){};

  virtual const char* getId() const override {
    return "nunchuk";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
};

#endif