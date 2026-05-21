#include "Arduino.h"
#include "Module.hpp"
#include "Common.hpp"
#include "LCDModule.hpp"
#include "JoystickModule.hpp"
#include "LEDMatrixModule.hpp"
#include "NunchukModule.hpp"
#include "RFIDModule.hpp"
#include <SPI.h>

/*static const pin_t rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LCDModule lcd(rs, en, d4, d5, d6, d7);
JoystickModule joystick(1, 2, 0);

static const pin_t dInPin = 12, clkPin = 10, csPin = 11;
LEDMatrixModule ledMatrix(dInPin, clkPin, csPin);

NunchukModule nunchuk;*/

static const pin_t ssPin = 53, rstPin = 5;
RFIDModule rfid(ssPin, rstPin);

void setup() {
  MiniCom::begin(115200);
  MiniCom::debug = true;
  SPI.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  /*pinMode(rs, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);*/

  //ModuleRegistry::add(&lcd);
  //ModuleRegistry::add(&joystick);
  //joystick.setEnabled(false);
  //ModuleRegistry::add(&ledMatrix);
  //ModuleRegistry::add(&nunchuk);
  rfid.setDiscriminator(42);
  ModuleRegistry::add(&rfid);

  ModuleRegistry::begin();

  Serial.print('\n');
  ModuleRegistry::sendCapabilities();
}

void loop() {
  MiniCom::update();
  ModuleRegistry::update();
}
