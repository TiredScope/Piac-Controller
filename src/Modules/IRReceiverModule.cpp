#include "Arduino.h"
#include "IRReceiverModule.hpp"
#include "IRremote.hpp"

void IRReceiverModule::begin() {
  IrReceiver.begin(recvPin);
}

void IRReceiverModule::update() {
  // Received something?
  if (!IrReceiver.decode()) {
    return;
  }

  IRData data = IrReceiver.decodedIRData;
  if (data.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
    MiniCom::debugPrint("[IRReceiver] Got overflow");
    IrReceiver.resume();
    return;
  }


  if (data.flags & IRDATA_FLAGS_IS_REPEAT) {
    // Repeat?
    MiniCom::debugPrint("[IRReceiver] Received repeat");
    IrReceiver.resume();
    return;
  }

  if (data.protocol == UNKNOWN) {
    MiniCom::debugPrintf("[IRReceiver] Received noise or a message with unknown protocol: %d", data.protocol);
    IrReceiver.resume();
    return;
  }

  MessageBuilder::reset(MessageType::M_IRRECEIVER_DATA, getDiscriminator());
  MessageBuilder::putU16((uint16_t)data.protocol);
  MessageBuilder::putU8(data.flags);
  MessageBuilder::putU16(data.address);
  MessageBuilder::putU16(data.command);
  MessageBuilder::putU16(data.extra);
  MiniCom::send(MessageBuilder::build());

  IrReceiver.resume();
}

void IRReceiverModule::onMessage(Message) {
  // Nothing to handle
}