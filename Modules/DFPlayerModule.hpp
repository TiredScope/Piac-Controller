#ifndef _THERMISTORMODULE_H_
#define _THERMISTORMODULE_H_

#include "Arduino.h"
#include "../Module.hpp"
#include "../Common.hpp"

#include "DFRobotDFPlayerMini.h"

enum class DFPlayerEvent : uint8_t {
  NONE = 0x00,

  CARD_INSERTED = 0x01,
  CARD_REMOVED = 0x02,
  CARD_ONLINE = 0x03,
  USB_INSERTED = 0x04,
  USB_REMOVED = 0x05,
  PLAY_FINISHED = 0x06,

  ERROR_TIMEOUT = 0x80,
  ERROR_NO_CARD = 0x81,
  ERROR_FILE_NOT_FOUND = 0x82,  // OOB or not found
  ERROR_OTHER = 0x83,
};

class DFPlayerModule : public Module {
private:
  DFRobotDFPlayerMini dfPlayer;
  Stream *serial;
public:
  DFPlayerModule(Stream *serial, uint8_t discriminator = DEFAULT_DISCRIMINATOR)
    : Module(discriminator), serial(serial){};

  virtual const char *getId() const override {
    return "dfplayer";
  }

  virtual void begin() override;
  virtual void update() override;
  virtual void onMessage(Message m) override;
  void sendState();
};

#ifdef INCLUDE_IMPLEMENTATION
#include "DFPlayerModule.cpp"
#endif

#endif