#include "Arduino.h"
#include "BME280Module.hpp"

#include <SPI.h>

void BME280Module::begin() {
  SPI.begin();

  if (!bme280.begin()) {
    MiniCom::debugPrint("Failed to initialize BME280");
  }
}

void BME280Module::update() {
  unsigned long now = millis();
  if (now - lastSent < reportingDelay) return;
  lastSent = now;

  float temperature = NAN;
  float humidity = NAN;
  float pressure = NAN;

  switch(bme280.chipModel())
   {
      case BME280::ChipModel_BME280:
        MiniCom::debugPrint("Found BME280 sensor! Success.");
        break;
      case BME280::ChipModel_BMP280:
        MiniCom::debugPrint("Found BMP280 sensor! No Humidity available.");
        break;
      default:
        MiniCom::debugPrint("Found UNKNOWN sensor! Error!");
   }

  bme280.read(pressure, temperature, humidity, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);

  MessageBuilder::reset(MessageType::M_BME280_VALUES, getDiscriminator());

  MessageBuilder::putF32(temperature);
  MessageBuilder::putF32(humidity);
  MessageBuilder::putF32(pressure);

  MiniCom::send(MessageBuilder::build());
}

void BME280Module::onMessage(Message m) {
  size_t idx;

  switch (m.getType()) {
    case MessageType::M_BME280_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
  }
}