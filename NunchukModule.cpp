#include "Arduino.h"
#include "NunchukModule.hpp"
#include "MiniCom.hpp"

void NunchukModule::begin() {
  nunchuk.begin();

  if(nunchuk.type == UnknownChuck) {
    nunchuk.type = NUNCHUCK;
  }
}

void NunchukModule::update() {
  unsigned long now = millis();
  if(now - lastSent < NUNCHUK_SEND_DELAY) return;
  lastSent = now;

  nunchuk.readData();

  MessageBuilder::reset(MessageType::M_NUNCHUK_VALUES, getDiscriminator());

  int joyX = nunchuk.getJoyX();
  int joyY = nunchuk.getJoyY();
  float rollAngle = nunchuk.getRollAngle();
  float pitchAngle = nunchuk.getPitchAngle();
  int accelX = nunchuk.getAccelX();
  int accelY = nunchuk.getAccelY();
  int accelZ = nunchuk.getAccelZ();
  bool buttonC = nunchuk.getButtonC();
  bool buttonZ = nunchuk.getButtonZ();

  MessageBuilder::putU8(joyX);
  MessageBuilder::putU8(joyY);
  MessageBuilder::putF32(rollAngle);
  MessageBuilder::putF32(pitchAngle);
  MessageBuilder::putU16(accelX);
  MessageBuilder::putU16(accelY);
  MessageBuilder::putU16(accelZ);
  MessageBuilder::putU8(buttonC ? 1 : 0);
  MessageBuilder::putU8(buttonZ ? 1 : 0);

  MiniCom::send(MessageBuilder::build());
}

void NunchukModule::onMessage(Message) {
  // Nothing to handle
}