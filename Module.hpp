#ifndef _MODULE_H_
#define _MODULE_H_

#ifndef MAX_MODULES
#define MAX_MODULES 16
#endif

#include "Arduino.h"
#include "MiniCom.hpp"

class Module {
private:
  bool enabled = true;
  uint8_t discriminator = DEFAULT_DISCRIMINATOR;
public:
  Module(uint8_t discriminator)
    : discriminator(discriminator) {}

  void setEnabled(bool enabled) {
    this->enabled = enabled;
  }

  bool isEnabled() const {
    return enabled;
  }

  void setDiscriminator(uint8_t discriminator) {
    this->discriminator = discriminator;
  }

  uint8_t getDiscriminator() const {
    return discriminator;
  }

  virtual const char *getId() const = 0;
  virtual void begin() = 0;
  virtual void update() = 0;
  virtual void onMessage(Message m) = 0;
};

class ModuleRegistry {
private:
  static Module *modules[MAX_MODULES];
  static size_t moduleCount;

  static void handleMessage(Message m);
public:
  static void begin();
  static void update();

  static void add(Module *module);
  static Module *find(const char *name, const uint8_t discriminator = DEFAULT_DISCRIMINATOR);

  static void sendCapabilities();
};

#endif