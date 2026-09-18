#include "Arduino.h"
#include "DFPlayerModule.hpp"

void DFPlayerModule::begin() {
  if (!dfPlayer.begin(*serial)) {
    MiniCom::debugPrint("Failed to intiialize DFPlayer");
    return;
  }

  dfPlayer.setTimeOut(500);
  dfPlayer.volume(10);
}

void DFPlayerModule::update() {
  if (!dfPlayer.available()) {
    return;
  }

  uint8_t type = dfPlayer.readType();
  uint16_t value = dfPlayer.read();

  DFPlayerEvent event = DFPlayerEvent::NONE;
  switch (type) {
    case TimeOut:
      {
        event = DFPlayerEvent::ERROR_TIMEOUT;
        break;
      }
    case WrongStack:
      {
        event = DFPlayerEvent::ERROR_OTHER;
        break;
      }
    case DFPlayerCardInserted:
      {
        event = DFPlayerEvent::CARD_INSERTED;
        break;
      }
    case DFPlayerCardRemoved:
      {
        event = DFPlayerEvent::CARD_REMOVED;
        break;
      }
    case DFPlayerCardOnline:
      {
        event = DFPlayerEvent::CARD_ONLINE;
        break;
      }
    case DFPlayerUSBInserted:
      {
        event = DFPlayerEvent::USB_INSERTED;
        break;
      }
    case DFPlayerUSBRemoved:
      {
        event = DFPlayerEvent::USB_REMOVED;
        break;
      }
    case DFPlayerPlayFinished:
      {
        event = DFPlayerEvent::PLAY_FINISHED;
        break;
      }
    case DFPlayerError:
      {
        switch (value) {
          case Busy:
            {
              event = DFPlayerEvent::ERROR_NO_CARD;
              break;
            }
          case FileIndexOut:
          case FileMismatch:
            {
              event = DFPlayerEvent::ERROR_FILE_NOT_FOUND;
              break;
            }
          default:
            {
              event = DFPlayerEvent::ERROR_OTHER;
              break;
            }
        }
      }
    default: break;
  }

  if (event != DFPlayerEvent::NONE) {
    MessageBuilder::reset(MessageType::M_DFPLAYER_EVENT, getDiscriminator());

    MessageBuilder::putU8((uint8_t)event);

    MiniCom::send(MessageBuilder::build());
  }
}

void DFPlayerModule::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_DFPLAYER_PLAY:
      {
        uint16_t file = m.getU16(idx);
        dfPlayer.play(file);
        break;
      }
    case MessageType::M_DFPLAYER_PLAY_FOLDER:
      {
        uint8_t folder = m.getU8(idx);
        uint16_t file = m.getU16(idx);
        dfPlayer.playLargeFolder(folder, file);
        break;
      }
    case MessageType::M_DFPLAYER_NEXT:
      {
        dfPlayer.next();
        break;
      }
    case MessageType::M_DFPLAYER_PREVIOUS:
      {
        dfPlayer.previous();
        break;
      }
    case MessageType::M_DFPLAYER_VOLUME:
      {
        uint8_t volume = m.getU8(idx);
        dfPlayer.volume(volume);
        break;
      }
    case MessageType::M_DFPLAYER_LOOP:
      {
        bool loop = m.getU8(idx) != 0;
        if (loop) {
          dfPlayer.enableLoop();
        } else {
          dfPlayer.disableLoop();
        }
        break;
      }
    case MessageType::M_DFPLAYER_LOOP_ONE:
      {
        uint16_t file = m.getU16(idx);
        dfPlayer.loop(file);
        break;
      }
    case MessageType::M_DFPLAYER_LOOP_FOLDER:
      {
        uint8_t folder = m.getU8(idx);
        dfPlayer.loopFolder(folder);
        break;
      }
    case MessageType::M_DFPLAYER_PAUSE:
      {
        dfPlayer.pause();
      }
    case MessageType::M_DFPLAYER_START:
      {
        dfPlayer.start();
        break;
      }
    case MessageType::M_DFPLAYER_RANDOM_ALL:
      {
        dfPlayer.randomAll();
        break;
      }
    case MessageType::M_DFPLAYER_EQ:
      {
        uint8_t eq = m.getU8(idx);
        dfPlayer.EQ(eq);
        break;
      }
    case MessageType::M_DFPLAYER_QUERY_STATE:
      {
        sendState();
        break;
      }
    default: break;
  }
}

void DFPlayerModule::sendState() {
  uint16_t state = dfPlayer.readState();
  uint8_t volume = dfPlayer.readVolume();
  uint8_t eq = dfPlayer.readEQ();
  uint16_t currentFile = dfPlayer.readCurrentFileNumber();
  uint16_t fileCount = dfPlayer.readFileCounts();
  uint8_t folderCount = dfPlayer.readFolderCounts();

  MessageBuilder::reset(MessageType::M_DFPLAYER_STATE, getDiscriminator());

  MessageBuilder::putU16(state);
  MessageBuilder::putU8(volume);
  MessageBuilder::putU8(eq);
  MessageBuilder::putU16(currentFile);
  MessageBuilder::putU16(fileCount);
  MessageBuilder::putU8(folderCount);

  for (uint8_t folder = 0; folder < folderCount; folder++) {
    MessageBuilder::putU16(dfPlayer.readFileCountsInFolder(folder));
  }

  MiniCom::send(MessageBuilder::build());
}