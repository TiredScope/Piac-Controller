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

  bme280.read(pressure, temperature, humidity, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);

  MessageBuilder::reset(MessageType::M_BME280_VALUES, getDiscriminator());

  MessageBuilder::putF32(temperature);
  MessageBuilder::putF32(humidity);
  MessageBuilder::putF32(pressure);

  MiniCom::send(MessageBuilder::build());
}

void BME280Module::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_BME280_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
  }
}