#include "Arduino.h"
#include "BMP280Module.hpp"

void BMP280Module::begin() {
  Wire.begin();

  if (!bmp280.begin()) {
    MiniCom::debugPrint("Failed to initialize BMP280");
  }

  // TODO: add a method for changing these?
  bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                     Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                     Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                     Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                     Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void BMP280Module::update() {
  unsigned long now = millis();
  if (now - lastSent < reportingDelay) return;
  lastSent = now;

  float temp = bmp280.readTemperature();
  float pressure = bmp280.readPressure();

  MessageBuilder::reset(MessageType::M_BMP280_VALUES, getDiscriminator());

  MessageBuilder::putF32(temp);
  MessageBuilder::putF32(pressure);

  MiniCom::send(MessageBuilder::build());
}

void BMP280Module::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_BMP280_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
    default:
      {
        break;
      }
  }
}