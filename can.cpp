#include "can.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "fdcan.h"

using namespace std;

static unordered_map<uint16_t, CanRx*> can_mailboxes;
static unordered_set<uint16_t> can_masks;

void can_addMailbox(uint16_t id, uint16_t mask, CanRx* mailbox){
  if(id > 0x7FF){
    return;
  }
  if (can_mailboxes.find(id) != can_mailboxes.end()) {
    return;
  }

  can_mailboxes.insert({id & mask, mailbox});
  can_masks.insert(mask);

}

CanRx* can_getMailbox(uint16_t id){
  if(id > 0x7FF){
    return nullptr;
  }
  if (can_mailboxes.find(id) == can_mailboxes.end()) {
    for (auto mask : can_masks) {
      if ((id & mask) == mask) {
        return can_mailboxes.at(mask);
      }
    }
    return nullptr;
  }

  return can_mailboxes.at(id);
}

uint32_t can_processRxFifo() {
    static FDCAN_RxHeaderTypeDef RxHeader;

    uint8_t RxData[8];
    can_clearMailboxes();
    while (HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
        uint32_t id = RxHeader.Identifier;
        uint32_t dlc = RxHeader.DataLength;
        CanRx* this_mailbox = can_getMailbox(id);
        if(this_mailbox != nullptr){
            copy(RxData, RxData + dlc, this_mailbox->data);
            this_mailbox->isRecent = true;
            this_mailbox->dlc = dlc;
        }
    }
    // If error code is something other than the fifo being cleared, set fault
    if(hfdcan2.ErrorCode != HAL_FDCAN_ERROR_NONE && hfdcan2.ErrorCode != HAL_FDCAN_ERROR_FIFO_EMPTY){
        return hfdcan2.ErrorCode;
    }
    return 0;
}

void can_clearMailboxes(){
    for(auto& mailbox : can_mailboxes){
        mailbox.second->isRecent = false;
    }
}

uint32_t can_send(uint16_t id, uint8_t dlc, uint8_t data[8]) {
    static FDCAN_TxHeaderTypeDef TxHeader;
    TxHeader.Identifier = id;
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = dlc;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;

    if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, data) != HAL_OK){
        return hfdcan2.ErrorCode;
    }
    return 0;
}

uint64_t can_readBytes(const uint8_t data[8], uint8_t start_byte, uint8_t end_byte){
  if (start_byte > end_byte || end_byte > 7 || start_byte > 7) {
    return 0;
  }
  uint64_t value = 0;
  for(int i = start_byte; i <= end_byte; i++){
    value |= static_cast<uint64_t>(data[i]) << (8 * (i - start_byte));
  }
  return value;
}

void can_writeBytes(uint8_t data[8], uint8_t start_byte, uint8_t end_byte, uint64_t value){
  if (start_byte > end_byte || end_byte > 7 || start_byte > 7) {
    return;
  }
  for(int i = start_byte; i <= end_byte; i++){
    data[i] = static_cast<uint8_t>(value >> (8 * (i - start_byte)));
  }
}
