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
} CanInbox;

typedef struct CanOutbox {
  bool isRecent = false;
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
 * @param id ID of the CAN packet you want to add
 * @param inbox Pointer to where the incoming packet is stored.
 */
void can_addInbox(uint32_t id, CanInbox *inbox);

/**
 * Designate all received packets with the given ID range to the be stored in the given mailbox range.\n
 * @param id ID of the CAN packet you want to add
 * @param inboxes Pointer to an array of CanInbox
 */
void can_addInboxes(uint32_t idLow, uint32_t idHigh, CanInbox *inboxes);

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

#endif //LONGHORN_LIBRARY_2024_CAN_H
