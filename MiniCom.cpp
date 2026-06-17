#include "MiniCom.hpp"

#include <Arduino.h>
#include <string.h>

#define FLOAT_BYTES (sizeof(float) / sizeof(uint8_t))

MessageType MessageBuilder::type;
uint8_t MessageBuilder::discriminator;
uint8_t MessageBuilder::buf[MAX_MESSAGE_LENGTH];
size_t MessageBuilder::idx;

void MessageBuilder::reset(MessageType type, uint8_t discriminator) {
  MessageBuilder::type = type;
  MessageBuilder::idx = 0;
  MessageBuilder::discriminator = discriminator;
}

bool MessageBuilder::putI8(int8_t value) {
  return putU8((uint8_t)value);
}

bool MessageBuilder::putU8(uint8_t value) {
  if (idx + 1 > MAX_MESSAGE_LENGTH) return false;
  buf[idx] = value;
  idx++;
  return true;
}

bool MessageBuilder::putI16(int16_t value) {
  return putU16((uint16_t)value);
}

bool MessageBuilder::putU16(uint16_t value) {
  if (idx + 2 > MAX_MESSAGE_LENGTH) return false;

  buf[idx] = (value >> 8) & 0xFF;
  buf[idx + 1] = value & 0xFF;

  idx += 2;
  return true;
}

bool MessageBuilder::putI32(int32_t value) {
  return putU32((uint32_t)value);
}

bool MessageBuilder::putU32(uint32_t value) {
  if (idx + 4 > MAX_MESSAGE_LENGTH) return false;

  buf[idx] = (value >> 24) & 0xFF;
  buf[idx + 1] = (value >> 16) & 0xFF;
  buf[idx + 2] = (value >> 8) & 0xFF;
  buf[idx + 3] = value & 0xFF;

  idx += 4;
  return true;
}

bool MessageBuilder::putF32(float value) {
  if (idx + FLOAT_BYTES > MAX_MESSAGE_LENGTH) return false;

  uint32_t binary;
  memcpy(&binary, &value, FLOAT_BYTES);
  return putU32(binary);
}

bool MessageBuilder::putString(const char *value) {
  size_t len = strlen(value) + 1;
  if (idx + len > MAX_MESSAGE_LENGTH) return false;

  memcpy(buf + idx, value, len);
  idx += len;
  return true;
}

bool MessageBuilder::putBytes(const char *value, size_t len) {
  if (idx + len > MAX_MESSAGE_LENGTH) return false;

  memcpy(buf + idx, value, len);
  idx += len;
  return true;
}

Message MessageBuilder::build() {
  return Message(type, discriminator, buf, idx);
}

int8_t Message::getI8(size_t &idx) const {
  return (int8_t)getU8(idx);
}

uint8_t Message::getU8(size_t &idx) const {
  if (idx + 1 > length) return 0;

  uint8_t value = data[idx];

  idx++;
  return value;
}

int16_t Message::getI16(size_t &idx) const {
  return (int16_t)getU16(idx);
}

uint16_t Message::getU16(size_t &idx) const {
  if (idx + 2 > length) return 0;

  uint16_t value = ((uint32_t)data[idx] << 8)
                   | ((uint32_t)data[idx + 1]);

  idx += 2;
  return value;
}

int32_t Message::getI32(size_t &idx) const {
  return (int32_t)getU32(idx);
}

uint32_t Message::getU32(size_t &idx) const {
  if (idx + 4 > length) return 0;

  uint32_t value = ((uint32_t)data[idx] << 24)
                   | ((uint32_t)data[idx + 1] << 16)
                   | ((uint32_t)data[idx + 2] << 8)
                   | ((uint32_t)data[idx + 3]);

  idx += 4;
  return value;
}

float Message::getF32(size_t &idx) const {
  if (idx + FLOAT_BYTES > length) return 0;

  uint32_t binary = getU32(idx);
  float value;
  memcpy(&value, &binary, FLOAT_BYTES);
  return value;
}

const char *Message::getString(size_t &idx) const {
  if (idx >= length) return nullptr;

  const char *str = (const char *)(data + idx);
  size_t len = strnlen(str, length - idx);
  if (len == length - idx) return nullptr;  // Reached end of data without terminating NULL character

  idx += len + 1;  // +1 for terminating NULL character
  return str;
}

const char *Message::getBytes(size_t &idx, size_t len) const {
  if (idx + len >= length) return nullptr;

  const char *str = (const char *)(data + idx);
  idx += len;
  return str;
}



unsigned long MiniCom::baud;
uint8_t MiniCom::buf[MAX_MESSAGE_LENGTH];
size_t MiniCom::bufIdx;
bool MiniCom::esc;
bool MiniCom::debug;
handler_func_t MiniCom::handler;

void MiniCom::parseMessage(const uint8_t *bytes, size_t length) {
  if (length < 4) return;

  MessageType type = (MessageType)(((uint16_t)bytes[0] << 8)
                                   | (uint16_t)bytes[1]);
  uint8_t discriminator = (uint8_t)bytes[2];
  checksum_t checksum = (checksum_t)bytes[length - 1];
  const uint8_t *data = bytes + 3;
  size_t dataLength = length - 4;

  checksum_t calculatedChecksum = 0;
  for (size_t i = 0; i < dataLength; i++) {
    checksum ^= data[i];
  }

  if (checksum != calculatedChecksum) {
    // TODO: handle invalid messages on the protocol level?
    MiniCom::debugPrintf("[MiniCom] Message checksum failure: %d != %d", (int)checksum, (int)calculatedChecksum);
    return;
  }

  Message msg(type, discriminator, data, dataLength);

  if (msg.getType() == MessageType::M_PING) {
    Message ping = Message(MessageType::M_PING, msg.getDiscriminator(), nullptr, 0);
    MiniCom::send(ping);
  }

  if (handler) handler(msg);
}

void MiniCom::update() {
  while (Serial.available() > 0) {
    uint8_t ch = (uint8_t)Serial.read();
    if (bufIdx >= MAX_MESSAGE_LENGTH) continue;
    if (!esc && ch == ESC_CHAR) {
      esc = true;
      continue;
    }

    if (!esc && ch == END_CHAR) {
      // End of message
      parseMessage(buf, bufIdx);
      memset(buf, 0, sizeof(buf));
      bufIdx = 0;
      continue;
    }

    buf[bufIdx] = ch;
    bufIdx++;
    esc = false;
  }
}

void MiniCom::writeEscaped(uint8_t byte) {
  if (byte == ESC_CHAR || byte == END_CHAR) {
    Serial.write(ESC_CHAR);
  }

  Serial.write(byte);
}

void MiniCom::send(Message m) {
  uint16_t type = (uint16_t)m.getType();
  writeEscaped((type >> 8) & 0xFF);
  writeEscaped(type & 0xFF);

  uint8_t discriminator = m.getDiscriminator();
  writeEscaped(discriminator);

  checksum_t checksum = 0;

  const uint8_t *data = m.getData();
  for (size_t i = 0; i < m.getLength(); i++) {
    checksum ^= data[i];
    writeEscaped(data[i]);
  }

  writeEscaped((uint8_t)checksum);
  Serial.write(END_CHAR);
}

void MiniCom::debugPrint(const char *str) {
  if (!MiniCom::debug) return;

  MessageBuilder::reset(MessageType::M_DEBUG, DEFAULT_DISCRIMINATOR);
  MessageBuilder::putString(str);
  send(MessageBuilder::build());
}

void MiniCom::debugPrintf(const char *fmt, ...) {
  if (!MiniCom::debug) return;

  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(NULL, 0, fmt, args);
  va_end(args);

  if (len > 0) {
    va_start(args, fmt);

    char *buffer = (char *)malloc(len + 1);
    if (buffer) {
      vsnprintf(buffer, len + 1, fmt, args);
      MiniCom::debugPrint(buffer);
      free(buffer);
    }

    va_end(args);
  }
}