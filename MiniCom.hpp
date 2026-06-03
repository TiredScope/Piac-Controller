#include "Print.h"
#ifndef _MINICOM_H_
#define _MINICOM_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "Arduino.h"

#define ESC_CHAR 0x1B  // ASCII ESC
#define END_CHAR '\n'

#ifdef MAX_MESSAGE_LENGTH

#if MAX_MESSAGE_LENGTH > UINT8_MAX
#error Message length is too large
#endif  // MAX_MESSAGE_LENGTH > UINT8_MAX

#else

#define MAX_MESSAGE_LENGTH UINT8_MAX

#endif  // MAX_MESSAGE_LENGTH

#define DEFAULT_DISCRIMINATOR 0

enum class MessageType : uint16_t {
  M_NOP = 0x0000,
  M_PING = 0x0001,
  M_CAPABILITIES = 0x0002,
  M_ENABLE = 0x0003,
  M_DEBUG = 0x0004,

  // LCDModule
  M_LCD_SETTINGS = 0x0100,
  M_LCD_CLEAR = 0x0101,
  M_LCD_PRINT = 0x0102,

  // JoystickModule
  M_JOYSTICK_VALUES = 0x0200,

  // LEDMatrixModule
  M_LEDMATRIX_IMAGE = 0x0300,

  // NunchukModule
  M_NUNCHUK_VALUES = 0x0400,

  // RFIDModule
  M_RFID_SCANNED = 0x0500,
  M_RFID_SCAN_FAILED = 0x0501,

  // PulseSensorModule
  M_PULSESENSOR_HEARTBEAT = 0x0600,

  // PIRSensorModule
  M_PIRSENSOR_PRESENCE = 0x0700,

  // UltrasonicSensorModule
  M_ULTRASONICSENSOR_DISTANCE = 0x0800,

  // LightSensorModule
  M_LIGHTSENSOR_VALUE = 0x0900,

  // IRReceiverModule
  M_IRRECEIVER_DATA = 0x0A00,

  // ThermistorModule
  M_THERMISTOR_VALUE = 0x0B00,
};

class Message;

typedef uint8_t checksum_t;
typedef void (*handler_func_t)(Message m);

class MessageBuilder {
public:
  static MessageType type;
  static uint8_t buf[MAX_MESSAGE_LENGTH];
  static size_t idx;
  static uint8_t discriminator;

  static void reset(MessageType type, uint8_t discriminator);
  static bool putI8(int8_t value);
  static bool putU8(uint8_t value);
  static bool putI16(int16_t value);
  static bool putU16(uint16_t value);
  static bool putI32(int32_t value);
  static bool putU32(uint32_t value);
  static bool putF32(float value);
  static bool putString(const char *value);
  static bool putBytes(const char *bytes, size_t len);
  static Message build();
};

class Message {
private:
  MessageType type;
  uint8_t discriminator;
  const uint8_t *data;
  size_t length;

public:
  // Note: Message does not take ownership of data
  Message(MessageType type, uint8_t discriminator, const uint8_t *data, size_t length)
    : type(type), discriminator(discriminator), data(data), length(length){};

  MessageType getType() const {
    return type;
  }

  uint8_t getDiscriminator() const {
    return discriminator;
  }

  const uint8_t *getData() const {
    return data;
  }

  size_t getLength() const {
    return length;
  }

  bool isEnd(size_t idx) const {
    return idx >= length;
  };

  int8_t getI8(size_t &idx) const;
  uint8_t getU8(size_t &idx) const;
  int16_t getI16(size_t &idx) const;
  uint16_t getU16(size_t &idx) const;
  int32_t getI32(size_t &idx) const;
  uint32_t getU32(size_t &idx) const;
  float getF32(size_t &idx) const;
  const char *getString(size_t &idx) const;
  const char *getBytes(size_t &idx, size_t len) const;
};

class MiniCom {
private:
  static unsigned long baud;
  static uint8_t buf[MAX_MESSAGE_LENGTH];
  static size_t bufIdx;
  static bool esc;

  static void parseMessage(const uint8_t *bytes, size_t length);
  static void writeEscaped(uint8_t byte);

public:
  static bool debug;
  static handler_func_t handler;

  static void begin(unsigned long baud) {
    MiniCom::baud = baud;
    memset(MiniCom::buf, 0, sizeof(MiniCom::buf));
    MiniCom::bufIdx = 0;
    MiniCom::esc = false;
    MiniCom::debug = false;
    MiniCom::handler = nullptr;
    Serial.begin(baud);
  };

  static void update();
  static void send(Message m);

  static void debugPrint(const char *str);
  static void debugPrintf(const char *fmt, ...);
};

#endif