#include "angel_can.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

static unordered_map<uint32_t, CanRx *> can_mailboxes;
static unordered_map<uint32_t, uint32_t> can_periodic_timers;
static unordered_map<uint32_t, uint32_t> can_periodic_curr_time;
static CAN_HANDLE *canHandleTypeDef;

/**
 * Get the period associated with the ID
 * @param id ID of the CAN packet.
 * @return Period of the CAN packet in milliseconds, returns 0 if it doesn't exist
 */
static uint32_t can_getTimer(uint32_t id);

/**
 * Get the mailbox associated with the ID
 * @param id ID of the CAN packet.
 * @return Pointer to the associated mailbox, returns null if it doesn't exist
 */
static CanRx* can_getMailbox(uint32_t id);

/**
 * Get the current time associated with the ID
 * @param id ID of the CAN packet.
 * @return Current time of the CAN packet in milliseconds, returns 0 if it doesn't exist
 */
static uint32_t can_getCurrTime(uint32_t id);

#ifdef STM32H7A3xx
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
#endif

uint32_t can_init(CAN_HANDLE *handle) {
    canHandleTypeDef = handle;

#ifdef STM32H7A3xx
    return HAL_FDCAN_Start(canHandleTypeDef);
#endif
#ifdef STM32L431xx
    return HAL_CAN_Start(canHandleTypeDef);
#endif
}
void can_addTimer(uint32_t id, uint32_t period) {
    if (can_periodic_timers.find(id) != can_periodic_timers.end()) {
        return;
    }

    can_periodic_timers.insert({id, period});
    can_periodic_curr_time.insert({id, 0});
}

void can_addRangeTimers(uint32_t idLow, uint32_t idHigh, uint32_t period) {
    for(uint32_t i = idLow; i <= idHigh; i++) {
        can_addTimer(i, period);
    }
}

void can_addMailbox(uint32_t id, CanRx *mailbox) {
    if (can_mailboxes.find(id) != can_mailboxes.end()) {
        return;
    }

    can_mailboxes.insert({id, mailbox});
}

void can_addMailboxes(uint32_t idLow, uint32_t idHigh, CanRx *mailboxes) {
    for(uint32_t i = idLow; i <= idHigh; i++) {
        can_addMailbox(i, &mailboxes[i - idLow]);
    }
}

static uint32_t can_getCurrTime(uint32_t id) {
    if (can_periodic_curr_time.find(id) == can_periodic_curr_time.end()) {
        return 0;
    } else {
        return can_periodic_curr_time.at(id);
    }
}

static uint32_t can_getTimer(uint32_t id) {
    if (can_periodic_timers.find(id) == can_periodic_timers.end()) {
        return 0;
    } else {
        return can_periodic_timers.at(id);
    }
}

static CanRx *can_getMailbox(uint32_t id) {
    if (can_mailboxes.find(id) == can_mailboxes.end()) {
        return nullptr;
    } else {
        return can_mailboxes.at(id);
    }
}

uint32_t can_processRxFifo() {
#ifdef STM32H7A3xx
    static FDCAN_RxHeaderTypeDef RxHeader;
    static uint8_t RxData[8];

    while (HAL_FDCAN_GetRxMessage(canHandleTypeDef, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
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
    if (canHandleTypeDef->ErrorCode != HAL_FDCAN_ERROR_NONE && canHandleTypeDef->ErrorCode != HAL_FDCAN_ERROR_FIFO_EMPTY) {
        return canHandleTypeDef->ErrorCode;
    }
#endif
#ifdef STM32L431xx
    static CAN_RxHeaderTypeDef RxHeader;
    static uint8_t RxData[8];
    while(HAL_CAN_GetRxFifoFillLevel(canHandleTypeDef, CAN_RX_FIFO0)) {
        if(HAL_CAN_GetRxMessage(canHandleTypeDef, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
            uint32_t id = (RxHeader.IDE == CAN_ID_EXT) ? RxHeader.ExtId : RxHeader.StdId;
            uint32_t dlc = RxHeader.DLC;
            CanRx *this_mailbox = can_getMailbox(id);
            if (this_mailbox != nullptr) {
                copy(RxData, RxData + dlc, this_mailbox->data);
                this_mailbox->isRecent = true;
                this_mailbox->dlc = dlc;
            }
        } else {
            return canHandleTypeDef->ErrorCode;
        }
    }
#endif
    return 0;
}

void can_clearMailboxes() {
    for (auto &mailbox: can_mailboxes) {
        mailbox.second->isRecent = false;
    }
}

uint32_t can_send(uint32_t id, uint8_t dlc, uint8_t* data) {
#ifdef STM32H7A3xx
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

    if (HAL_FDCAN_AddMessageToTxFifoQ(canHandleTypeDef, &TxHeader, data) != HAL_OK) {
        return canHandleTypeDef->ErrorCode;
    }
#endif
#ifdef STM32L431xx
    static CAN_TxHeaderTypeDef TxHeader;
    if(id > 0x7FF) {
        TxHeader.IDE = CAN_ID_EXT;
        TxHeader.ExtId = id;
    } else {
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.StdId = id;
    }
    TxHeader.DLC = dlc;
    TxHeader.RTR = CAN_RTR_DATA;

    static uint32_t TxMailbox;
    if (HAL_CAN_AddTxMessage(canHandleTypeDef, &TxHeader, data, &TxMailbox) != HAL_OK) {
        return canHandleTypeDef->ErrorCode;
    }
#endif

    return 0;
}

uint32_t can_send_periodic(uint32_t id, uint8_t dlc, uint8_t data[8], uint32_t delta){
    uint32_t curr_time = can_getCurrTime(id);
    uint32_t timer = can_getTimer(id);
    if (timer == 0) { // No timer set
        return can_send(id, dlc, data);
    } else if (curr_time + delta >= timer) { // Timer expired
        can_periodic_curr_time[id] = 0;
        return can_send(id, dlc, data);
    } else { // Timer not expired
        can_periodic_curr_time[id] += delta;
        return 0;
    }
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
