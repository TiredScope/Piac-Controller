#include "SensirionErrors.h"
#include "Arduino.h"
#include "SCD41Module.hpp"
#include "Wire.h"

static char errorMessage[64];

void SCD41Module::begin() {
  Wire.begin();
  scd41.begin(Wire, SCD41_I2C_ADDR_62);

  int16_t error = scd41.wakeUp();
  if (error != 0) {
    errorToString(error, errorMessage, sizeof errorMessage);
    MiniCom::debugPrintf("[SCD41] Error on wakeUp(): %s", errorMessage);
    return;
  }

  error = scd41.stopPeriodicMeasurement();
  if (error != 0) {
    errorToString(error, errorMessage, sizeof errorMessage);
    MiniCom::debugPrintf("[SCD41] Error on stopPeriodicMeasurement(): %s", errorMessage);
    return;
  }

  error = scd41.reinit();
  if (error != 0) {
    errorToString(error, errorMessage, sizeof errorMessage);
    MiniCom::debugPrintf("[SCD41] Error on reinit(): %s", errorMessage);
    return;
  }

  error = scd41.startPeriodicMeasurement();
  if (error != 0) {
    errorToString(error, errorMessage, sizeof errorMessage);
    MiniCom::debugPrintf("[SCD41] Error on startPeriodicMeasurement(): %s", errorMessage);
    return;
  }
}

void SCD41Module::update() {
  unsigned long now = millis();
  if (now - lastSent < reportingDelay) return;
  lastSent = now;

  bool ready = false;

  int16_t error = scd41.getDataReadyStatus(ready);
  if (error != 0) {
    errorToString(error, errorMessage, sizeof errorMessage);
    MiniCom::debugPrintf("[SCD41] Error on getDataReadyStatus(): %s", errorMessage);
    return;
  }

  if (!ready) {
    lastSent = now - reportingDelay + 100;  // Retry in 100ms
    return;
  }

  uint16_t co2 = 0;
  float temperature = 0;
  float humidity = 0;
  scd41.readMeasurement(co2, temperature, humidity);

  MessageBuilder::reset(MessageType::M_SCD41_VALUES, getDiscriminator());

  MessageBuilder::putU16(co2);
  MessageBuilder::putF32(temperature);
  MessageBuilder::putF32(humidity);

  MiniCom::send(MessageBuilder::build());
}

void SCD41Module::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_SCD41_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
  }
}