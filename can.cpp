#include "can.h"

std::unordered_map<uint16_t, CanRx*> can_mailboxes;

void can_addMailbox(uint16_t id, CanRx* mailbox){
  if(id > 0x7FF){
    return;
  }
  if (can_mailboxes.find(id) != can_mailboxes.end()) {
    return;
  }

  can_mailboxes[id] = mailbox;
}

void can_processRxFifo(){
  for(auto& [id, mailbox] : can_mailboxes){
    if(mailbox->isRecent){
      mailbox->isRecent = false;
    }
  }
}

void can_send(uint16_t id, uint8_t dlc, uint8_t data[8]);

template<typename T>
void read_packet(const uint8_t data[8], uint8_t start_byte, uint8_t end_byte, T& dest){
  if (start_byte > end_byte || end_byte > 7 || start_byte > 7) {
    return;
  }
  uint64_t value = 0;
  for(int i = start_byte; i <= end_byte; i++){
    value |= static_cast<uint64_t>(data[i]) << (8 * (i - start_byte));;
  }
  dest = static_cast<T>(value);
}

template<typename U>
void write_packet(uint8_t data[8], uint8_t start_byte, uint8_t end_byte, U value){
  if (start_byte > end_byte || end_byte > 7 || start_byte > 7) {
    return;
  }
  for(int i = start_byte; i <= end_byte; i++){
    data[i] = static_cast<uint8_t>(value >> (8 * (i - start_byte)));
  }
}
