#include "Arduino.h"
#include "SparkfunKeypadModule.hpp"

void SparkfunKeypadModule::begin() {
  // Nothing to do
  lastState = 0xFF;
}

void SparkfunKeypadModule::update() {
  char key = keypad.getKey();
  if (key) {
    MessageBuilder::reset(MessageType::M_SPARKFUN_KEYPAD_KEY, getDiscriminator());
    MessageBuilder::putU8(key);
    MiniCom::send(MessageBuilder::build());
  }
}

void SparkfunKeypadModule::onMessage(Message m) {
  // Nothing to handle
}