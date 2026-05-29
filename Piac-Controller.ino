#include "Arduino.h"
#include "Module.hpp"
#include "Common.hpp"

//#include "src/Modules/LCDModule.hpp"
//#include "src/Modules/JoystickModule.hpp"
//#include "src/Modules/LEDMatrixModule.hpp"
//#include "src/Modules/NunchukModule.hpp"
//#include "src/Modules/RFIDModule.hpp"
//#include "src/Modules/PulseSensorModule.hpp"
#include "src/Modules/PIRSensorModule.hpp"

/*static const pin_t rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LCDModule lcd(rs, en, d4, d5, d6, d7);
JoystickModule joystick(1, 2, 0);

static const pin_t dInPin = 12, clkPin = 10, csPin = 11;
LEDMatrixModule ledMatrix(dInPin, clkPin, csPin);

NunchukModule nunchuk;*/

//static const pin_t ssPin = 53, rstPin = 5;
//RFIDModule rfid(ssPin, rstPin);

//static const pin_t hearbeatPin = A0;
//PulseSensorModule pulseSensor(hearbeatPin, 550 /* threshold */);

static const pin_t pirPin = 2;
PIRSensorModule pirSensor(pirPin);

void setup() {
  MiniCom::begin(115200);
  MiniCom::debug = true;

  //ModuleRegistry::add(&lcd);
  //ModuleRegistry::add(&joystick);
  //ModuleRegistry::add(&ledMatrix);
  //ModuleRegistry::add(&nunchuk);
  //rfid.setDiscriminator(42);
  //ModuleRegistry::add(&rfid);
  //ModuleRegistry::add(&pulseSensor);
  ModuleRegistry::add(&pirSensor);

  ModuleRegistry::begin();

  Serial.print('\n');
  ModuleRegistry::sendCapabilities();
}

void loop() {
  MiniCom::update();
  ModuleRegistry::update();
}
