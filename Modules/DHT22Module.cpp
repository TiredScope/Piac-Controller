#include "Arduino.h"
#include "DHT22Module.hpp"

void DHT22Module::begin() {
  // Nothing to do
}

void DHT22Module::update() {
  unsigned long now = millis();
  if (now - lastSent < reportingDelay) return;
  lastSent = now;

  float temperature = dht22.getTemperature();
  float humidity = dht22.getHumidity();

  if(dht22.getLastError() != dht22.OK) {
    MiniCom::debugPrintf("[DHT22] Got error: %d", dht22.getLastError());
    return;
  }

  MessageBuilder::reset(MessageType::M_DHT22_VALUES, getDiscriminator());

  MessageBuilder::putF32(dht22.getTemperature());
  MessageBuilder::putF32(dht22.getHumidity());

  MiniCom::send(MessageBuilder::build());
}

void DHT22Module::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_DHT22_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
  }
}