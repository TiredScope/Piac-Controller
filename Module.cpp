#include "Module.hpp"

Module *ModuleRegistry::modules[MAX_MODULES] = { 0 };
size_t ModuleRegistry::moduleCount = 0;

void ModuleRegistry::handleMessage(Message m) {
  switch (m.getType()) {
    case MessageType::M_CAPABILITIES:
      {
        sendCapabilities();
        return;
      }
    case MessageType::M_ENABLE:
      {
        size_t idx = 0;
        const char *name = m.getString(idx);
        Module *module = ModuleRegistry::find(name, m.getDiscriminator());
        if (module == nullptr) {
          MiniCom::debugPrintf("Failed to disable module (%s @ %d), not found", name, m.getDiscriminator());
          return;
        }
        bool enable = m.getU8(idx) != 0;
        module->setEnabled(enable);
        MiniCom::debugPrintf("Module (%s @ %d) is now %s", name, module->getDiscriminator(), enable ? "enabled" : "disabled");
        return;
      }
    default: break;
  }

  for (Module *module : modules) {
    if (!module || !module->isEnabled()) continue;
    if(m.getDiscriminator() != DEFAULT_DISCRIMINATOR && module->getDiscriminator() != m.getDiscriminator()) continue;
    module->onMessage(m);
  }
}

void ModuleRegistry::begin() {
  MiniCom::handler = handleMessage;

  for (Module *module : modules) {
    if (!module) continue;
    module->begin();
  }
}

void ModuleRegistry::update() {
  for (Module *module : modules) {
    if (!module || !module->isEnabled()) continue;
    module->update();
  }
}

void ModuleRegistry::add(Module *module) {
  modules[moduleCount++] = module;
}

Module *ModuleRegistry::find(const char *name, const uint8_t discriminator) {
  for (Module *module : modules) {
    if (!module) continue;
    if (strcmp(module->getId(), name) != 0) continue;
    if (discriminator != DEFAULT_DISCRIMINATOR && module->getDiscriminator() != discriminator) continue;
    return module;
  }

  return nullptr;
}

void ModuleRegistry::sendCapabilities() {
  MessageBuilder::reset(MessageType::M_CAPABILITIES, DEFAULT_DISCRIMINATOR);
  for (Module *module : modules) {
    if (!module) continue;
    MessageBuilder::putString(module->getId());
    MessageBuilder::putU8(module->getDiscriminator());
  }

  MiniCom::send(MessageBuilder::build());
}