#ifndef LONGHORN_LIBRARY_2024_CAN_H
#define LONGHORN_LIBRARY_2024_CAN_H

#include <stdint.h>
#include "angel_can_ids.h"

#ifdef STM32H7A3xxQ
#define H7_SERIES
#endif
#ifdef STM32H7A3xx
#define H7_SERIES
#endif
#ifdef H7_SERIES
#include "fdcan.h"
#define CAN_HANDLE FDCAN_HandleTypeDef
#endif
#ifdef STM32L431xx
#include "can.h"
#define CAN_HANDLE CAN_HandleTypeDef
#endif

typedef struct CanInbox {
  bool isRecent = false;
  uint8_t dlc = 0;
  uint8_t data[8] = {};
  float ageSinceRx = 0;
  float timeLimit = 0;
  bool isTimeout = false;
} CanInbox;

typedef struct CanOutbox {
  bool isRecent = false; // obsolete
  uint8_t dlc = 0;
  uint8_t data[8] = {};
  float period = 1000000.0f;
  float _timer = 0;
} CanOutbox;

void can_init(CAN_HANDLE *handle);

/**
 * Add a CAN outbox to be sent periodically.\n
 * The period is the rate at which CAN packets of this ID are sent. \n
 * @param id ID of the CAN packet you want to add
 * @param period in seconds
 * @param outbox Pointer to CanOutbox struct
 */
void can_addOutbox(uint32_t id, float period, CanOutbox *outbox);

/**
 * Add a range CAN outboxes to be sent periodically.\n
 * The period is the rate at which CAN packets of these IDs are sent. \n
 * @param idLow ID of the CAN packet you want to add
 * @param idHigh ID of the CAN packet you want to add
 * @param period in seconds
 * @param outboxes Pointer to array of CanOutbox structs
 */
void can_addOutboxes(uint32_t idLow, uint32_t idHigh, float period, CanOutbox *outboxes);

/**
 * Designate all received packets with the given ID to the be stored in the given mailbox.\n
 * Also sets up timeout feature if a time limit is given.\n
 * @param id ID of the CAN packet you want to add
 * @param inbox Pointer to where the incoming packet is stored.
 * @param timeoutLimit time where TimeOut is triggered
 */
void can_addInbox(uint32_t id, CanInbox *inbox, float timeoutLimit = 0);

/**
 * Designate all received packets with the given ID range to the be stored in the given mailbox range.\n
 * Also sets up timeout feature if a time limit is given.\n
 * @param id ID of the CAN packet you want to add
 * @param inboxes Pointer to an array of CanInbox
 * @param timeoutLimit time where TimeOut is triggered for all mailboxes
 */
void can_addInboxes(uint32_t idLow, uint32_t idHigh, CanInbox *inboxes, float timeoutLimit = 0);

/**
 * Update the corresponding mailboxes, emptying the RxFifo.
 */
uint32_t can_periodic(float deltaTime);

/**
 * Attempts to send a CAN packet. There are no restrictions at this point, so only use if necessary.
 * @param id ID of the CAN packet
 * @param dlc Length of the CAN packet
 * @param data Data of the CAN packet
 * @param delta Time in milliseconds between each packet
 * @return 0 if successful, 1 if error, 2 if unsuccessful send
 */
uint32_t can_send(uint32_t id, uint8_t dlc, uint8_t data[8]);

/**
 * Read a parameter from the packet, specifying which bytes to read.\n
 * This assumes that the packets are little endian, which is the standard set in CM200dz.\n
 * @param data Data of the CAN packet
 * @param start_byte First byte to read
 * @param end_byte Last byte to read
 * @return Value of the parameter
 * @note This supports up to 64 bit integers, and the result must be cast into other types by you.
 */
uint64_t can_readBytes(const uint8_t data[8], uint8_t start_byte, uint8_t end_byte);

/**
 * Write a parameter to the packet, specifying which bytes to write.\n
 * This assumes that the packets are little endian, which is the standard set in CM200dz.\n
 * @param data Data of the CAN packet
 * @param start_byte First byte to write
 * @param end_byte Last byte to write
 * @param value Value of the parameter
 */
void can_writeBytes(uint8_t data[8], uint8_t start_byte, uint8_t end_byte, uint64_t value);

/**
 * Read a integral value from the packets, based on the given type
 * @param Inbox Inbox of the CAN packet, which stores the data and dlc
 * @param start_byte First byte to read from
 */
template <typename T>
T can_readInt(CanInbox *inbox, size_t start_byte) {
  return *reinterpret_cast<T *>(inbox->data + start_byte);
}

/**
 * Write a integral value to the packets, based on the given type
 * @param Outbox Outbox of the CAN packet, which stores the data and dlc
 * @param start_byte First byte to write to
 * @param value Value to write
 */
template <typename T>
void can_writeInt(CanOutbox *outbox, size_t start_byte, T value) {
  *reinterpret_cast<T *>(outbox->data + start_byte) = value;
}

/**
 * Read a floating point value from the packet,
 * You specify the type to convert the bytes into before converting to float
 * Converting to float requires multiplying the given type by precision
 * @param Inbox Inbox of the CAN packet, which stores the data and dlc
 * @param start_byte First byte to read from
 * @param precision The amount of decimal places to read
 */
template <typename T>
float can_readFloat(CanInbox *inbox, size_t start_byte, float precision = 1.0f) {
  T rvalue = *reinterpret_cast<T *>(inbox->data + start_byte);
  return static_cast<float>(rvalue * precision);
}

/**
 * Write a floating point value to the packet.
 * You specify the type to convert the float into before converting to bytes.
 * Converting to bytes requires dividing the float by precision
 * @param Outbox Outbox of the CAN packet, which stores the data and dlc
 * @param start_byte First byte to write to
 * @param value Value to write
 * @param precision The amount of decimal places to write
 */
template <typename T>
void can_writeFloat(CanOutbox *outbox, size_t start_byte, float value, float precision = 1.0f) {
  T wvalue = static_cast<T>(value / precision);
  *reinterpret_cast<T *>(outbox->data + start_byte) = wvalue;
}

#endif //LONGHORN_LIBRARY_2024_CAN_H
