#include "RFIDModule.hpp"

void RFIDModule::begin() {
  // RFID initialization
  spi.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  cardRead = false;
}

void RFIDModule::update() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    // No card present
    cardRead = false;
    return;
  }

  if (cardRead) {
    // Card was just read previously, ignore
    MiniCom::debugPrint("[RFID] Card read already");
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    // Read failed
    MiniCom::send(Message(MessageType::M_RFID_SCAN_FAILED, getDiscriminator(), nullptr, 0));
    MiniCom::debugPrint("[RFID] Scan failed");
    return;
  }

  cardRead = true;

  MessageBuilder::reset(MessageType::M_RFID_SCANNED, getDiscriminator());
  MessageBuilder::putU8((uint8_t)mfrc522.uid.size);
  MessageBuilder::putBytes((const char *)mfrc522.uid.uidByte, (size_t)mfrc522.uid.size);
  MiniCom::send(MessageBuilder::build());

  mfrc522.PICC_HaltA();
}

void RFIDModule::onMessage(Message) {
  // Pseudo-code
  /*if(m.type == M_RFID_SET_STATE) {
    enabled = m.data[0];
  }*/
}