#include "can.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

static unordered_map<uint32_t, CanRx *> can_mailboxes;
static FDCAN_HandleTypeDef *fdcanHandle;

uint8_t dlc_to_num(uint32_t dlc) {
    switch (dlc) {
        case FDCAN_DLC_BYTES_0:
            return 0;
        case FDCAN_DLC_BYTES_1:
            return 1;
        case FDCAN_DLC_BYTES_2:
            return 2;
        case FDCAN_DLC_BYTES_3:
            return 3;
        case FDCAN_DLC_BYTES_4:
            return 4;
        case FDCAN_DLC_BYTES_5:
            return 5;
        case FDCAN_DLC_BYTES_6:
            return 6;
        case FDCAN_DLC_BYTES_7:
            return 7;
        case FDCAN_DLC_BYTES_8:
            return 8;
        default:
            return 0;
    }
}

uint32_t num_to_dlc(uint8_t num) {
    switch (num) {
        case 0:
            return FDCAN_DLC_BYTES_0;
        case 1:
            return FDCAN_DLC_BYTES_1;
        case 2:
            return FDCAN_DLC_BYTES_2;
        case 3:
            return FDCAN_DLC_BYTES_3;
        case 4:
            return FDCAN_DLC_BYTES_4;
        case 5:
            return FDCAN_DLC_BYTES_5;
        case 6:
            return FDCAN_DLC_BYTES_6;
        case 7:
            return FDCAN_DLC_BYTES_7;
        case 8:
            return FDCAN_DLC_BYTES_8;
        default:
            return FDCAN_DLC_BYTES_0;
    }
}

uint32_t can_init(FDCAN_HandleTypeDef *handle) {
    fdcanHandle = handle;
    return HAL_FDCAN_Start(fdcanHandle);
}

void can_addMailbox(uint32_t id, CanRx *mailbox) {
    if (can_mailboxes.find(id) != can_mailboxes.end()) {
        return;
    }

    can_mailboxes.insert({id, mailbox});
}

static CanRx *can_getMailbox(uint32_t id) {
    if (can_mailboxes.find(id) == can_mailboxes.end()) {
        return nullptr;
    } else {
        return can_mailboxes.at(id);
    }
}

uint32_t can_processRxFifo() {
    static FDCAN_RxHeaderTypeDef RxHeader;

    uint8_t RxData[8];
    can_clearMailboxes();
    while (HAL_FDCAN_GetRxMessage(fdcanHandle, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
        uint32_t id = RxHeader.Identifier;
        uint8_t dlc = dlc_to_num(RxHeader.DataLength);
        CanRx *this_mailbox = can_getMailbox(id);
        if (this_mailbox != nullptr) {
            copy(RxData, RxData + dlc, this_mailbox->data);
            this_mailbox->isRecent = true;
            this_mailbox->dlc = dlc;
        }
    }
    // If error code is something other than the fifo being cleared, set fault
    if (fdcanHandle->ErrorCode != HAL_FDCAN_ERROR_NONE && fdcanHandle->ErrorCode != HAL_FDCAN_ERROR_FIFO_EMPTY) {
        return fdcanHandle->ErrorCode;
    }
    return 0;
}

void can_clearMailboxes() {
    for (auto &mailbox: can_mailboxes) {
        mailbox.second->isRecent = false;
    }
}

uint32_t can_send(uint32_t id, uint8_t dlc, uint8_t data[8]) {
    static FDCAN_TxHeaderTypeDef TxHeader;
    TxHeader.Identifier = id;
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = num_to_dlc(dlc);
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;

    if (HAL_FDCAN_AddMessageToTxFifoQ(fdcanHandle, &TxHeader, data) != HAL_OK) {
        return fdcanHandle->ErrorCode;
    }
    return 0;
}

uint64_t can_readBytes(const uint8_t data[8], uint8_t start_byte, uint8_t end_byte) {
    if (start_byte > end_byte || end_byte > 7 || start_byte > 7) {
        return 0;
    }
    uint64_t value = 0;
    for (int i = start_byte; i <= end_byte; i++) {
        value |= static_cast<uint64_t>(data[i]) << (8 * (i - start_byte));
    }
    return value;
}

void can_writeBytes(uint8_t data[8], uint8_t start_byte, uint8_t end_byte, uint64_t value) {
    if (start_byte > end_byte || end_byte > 7 || start_byte > 7) {
        return;
    }
    for (int i = start_byte; i <= end_byte; i++) {
        data[i] = static_cast<uint8_t>(value >> (8 * (i - start_byte)));
    }
}
