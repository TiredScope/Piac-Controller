#include "Arduino.h"
#include "Module.hpp"
#include "Common.hpp"

#define INCLUDE_IMPLEMENTATION  // Because the modules are (effectively) header-only libraries: The .cpp file is included when the define is present
//#include "Modules/LCDModule.hpp"
//#include "Modules/JoystickModule.hpp"
//#include "Modules/LEDMatrixModule.hpp"
//#include "Modules/NunchukModule.hpp"
//#include "Modules/RFIDModule.hpp"
//#include "Modules/PulseSensorModule.hpp"
//#include "Modules/PIRSensorModule.hpp"
//#include "Modules/UltrasonicSensorModule.hpp"
//#include "Modules/LightSensorModule.hpp"
//#include "Modules/IRReceiverModule.hpp"
//#include "Modules/ThermistorModule.hpp"
//#include "Modules/CircuitPlaygroundModule.hpp"
//#include "Modules/DHT22Module.hpp"
#include "Modules/MQ3Module.hpp"

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

//static const pin_t pirPin = 2;
//PIRSensorModule pirSensor(pirPin);

//static const pin_t echoPin = 11, trigPin = 12;
//UltrasonicSensorModule ultrasonicSensor(echoPin, trigPin);

//static const pin_t lightSensorPin = A0;
//LightSensorModule lightSensor(lightSensorPin);

//static const pin_t irRecvPin = 11;
//IRReceiverModule irReceiver(irRecvPin);

//static const pin_t thermistorPin = A0;
//ThermistorModule thermistor(thermistorPin);

//CircuitPlaygroundModule circuitPlayground;

//DHT22Module dht22(2);

pin_t mq3DPin = 13, mq3APin = A0;
MQ3Module mq3(mq3DPin, mq3APin);

void setup() {
  MiniCom::begin(115200);
  MiniCom::debug = true;

  //ModuleRegistry::add(&lcd);
  //ModuleRegistry::add(&joystick);
  //ModuleRegistry::add(&ledMatrix);
  //ModuleRegistry::add(&nunchuk);
  //ModuleRegistry::add(&rfid);
  //ModuleRegistry::add(&pulseSensor);
  //ModuleRegistry::add(&pirSensor);
  //ModuleRegistry::add(&ultrasonicSensor);
  //ModuleRegistry::add(&lightSensor);
  //ModuleRegistry::add(&irReceiver);
  //ModuleRegistry::add(&thermistor);
  //ModuleRegistry::add(&circuitPlayground);
  //ModuleRegistry::add(&dht22);
  ModuleRegistry::add(&mq3);

  ModuleRegistry::begin();

  Serial.print('\n');
  ModuleRegistry::sendCapabilities();
}

void loop() {
  MiniCom::update();
  ModuleRegistry::update();
}
