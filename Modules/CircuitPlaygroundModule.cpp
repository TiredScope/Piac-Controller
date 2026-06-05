#include "Arduino.h"
#include "CircuitPlaygroundModule.hpp"
#include "Adafruit_CircuitPlayground.h"

void CircuitPlaygroundModule::begin() {
}

void CircuitPlaygroundModule::update() {
  // Received something?
  //MessageBuilder::reset(MessageType::M_IRRECEIVER_DATA, getDiscriminator());
  //MessageBuilder::putU16((uint16_t)data.protocol);
  //MessageBuilder::putU8(data.flags);
  //MessageBuilder::putU16(data.address);
  //MessageBuilder::putU16(data.command);
  //MessageBuilder::putU16(data.extra);
  //MiniCom::send(MessageBuilder::build());
}

void CircuitPlaygroundModule::onMessage(Message) {
  // Nothing to handle
}