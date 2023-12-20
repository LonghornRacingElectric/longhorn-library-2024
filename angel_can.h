#ifndef LONGHORN_LIBRARY_2024_CAN_H
#define LONGHORN_LIBRARY_2024_CAN_H

#include <stdint.h>
#include "angel_can_ids.h"

#ifdef STM32H7A3xx
#include "fdcan.h"
#define CAN_HANDLE FDCAN_HandleTypeDef
#endif

#ifdef STM32L431xx
#include "can.h"
#define CAN_HANDLE CAN_HandleTypeDef
#endif

typedef struct CanRx {
    bool isRecent;
    uint8_t dlc;
    uint8_t data[8];
} CanRx;

uint32_t can_init(CAN_HANDLE* handle);

/**
 * Tell the CAN driver to associate every ID with a period (in ms).\n
 * The period is the maximum rate at which CAN packets of this ID can be pushed to the FIFO. \n
 * @param id ID of the CAN packet you want to add
 * @param period Period of the CAN packet in milliseconds
 */
void can_addTimer(uint32_t id, uint32_t period);

/**
 * Tell the CAN driver to associate the range of IDs with a period (in ms).\n
 * The period is the maximum rate at which CAN packets of this ID can be pushed to the FIFO. \n
 * @param idLow ID of the CAN packet you want to add
 * @param idHigh ID of the CAN packet you want to add
 * @param period Period of the CAN packet in milliseconds
 */
void can_addRangeTimers(uint32_t idLow, uint32_t idHigh, uint32_t period);

/**
 * Tell the CAN driver to copy all incoming packets with a given ID to the given mailbox address.\n
 * Mailbox must be pre-defined (i.e., declared in your code as "static CanRx <name>") .\n
 * @param id ID of the CAN packet you want to add
 * @param mailbox Pointer to where the incoming packet is stored.
 */
void can_addMailbox(uint32_t id, CanRx* mailbox);

/**
 * Tell the CAN driver to copy all incoming packets with a given ID range to the given mailbox array.\n
 * Mailbox is a pointer to an array of CanRx where the first element corresponds to the first ID in the range.\n
 * @param id ID of the CAN packet you want to add
 * @param mailbox Pointer to where the incoming packet is stored.
 */
void can_addMailboxes(uint32_t idLow, uint32_t idHigh, CanRx* mailboxes);

/**
 * Update the corresponding mailboxes, emptying the RxFifo.
 */
uint32_t can_processRxFifo();

/**
 * Clear the recency of the mailboxes.
 */
void can_clearMailboxes();

/**
 * Put a CAN packet in the Tx FIFO, which is guaranteed to be pushed onto the CAN BUS
 * @param id ID of the CAN packet
 * @param dlc Length of the CAN packet
 * @param data Data of the CAN packet
 * @return 0 if successful, 1 if unsuccessful
 */
uint32_t can_send(uint32_t id, uint8_t dlc, uint8_t* data);

/**
 * Attempts to put in a CAN packet in the Tx FIFO so that it is rate-limited.\n
 * The period of that rate is pre-determined by the ID through can_addTimer or can_addRangeTimers. \n
 * If no rate is set for the ID, the packet is sent without rate-limiting.\n
 * @param id ID of the CAN packet
 * @param dlc Length of the CAN packet
 * @param data Data of the CAN packet
 * @param delta Time in milliseconds between each packet
 * @return 0 if successful, 1 if error, 2 if unsuccessful send
 */
uint32_t can_send_periodic(uint32_t id, uint8_t dlc, uint8_t data[8], uint32_t delta);

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
