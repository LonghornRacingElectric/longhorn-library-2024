#ifndef LONGHORN_LIBRARY_2024_CAN_H
#define LONGHORN_LIBRARY_2024_CAN_H

#include <cstdint>
// CAN IDs
// Inverter predefined CAN IDs
#define INV_TEMP1_DATA 0x0A0 //Stores inverter module temperature
#define INV_TEMP3_DATA 0x0A2 //Stores motor temperature
#define INV_MOTOR_POSITIONS 0x0A5 //Stores motor position
#define INV_CURRENT 0x0A6 //Stores motor velocity
#define INV_VOLTAGE 0x0A7 //Stores motor voltage
#define INV_STATE_CODES 0x0AA //Stores inverter state codes
#define INV_FAULT_CODES 0x0AB //Stores inverter fault codes
#define INV_TORQUE_TIMER 0x0AC //Stores inverter torque results
#define INV_HIGH_SPEED_MSG 0x0B0 //Stores inverter high speed message
#define VCU_INV_COMMAND 0x0C0 //Stores inverter command
#define VCU_INV_PARAMS_REQUEST 0x0C1 //Sets inverter parameter r/w
#define INV_VCU_PARAMS_RESPONSE 0x0C2 //Responds back success of parameter r/w

// VCU CAN IDs
#define VCU_DASH_FAULT_INFO 0x110 //Sends out VCU fault info to dash

//Parameters to set to boards (unused currently)
#define VCU_HVC_PARAMS 0x0F0
#define VCU_PDU_PARAMS 0x0F1
#define VCU_WHS_PARAMS 0x0F2
#define VCU_UPR_PARAMS 0x0F3

// HVC CAN IDs
#define HVC_VCU_CELL_VOLTAGES_START 0x300 // 0x300-0x37F
#define HVC_VCU_CELL_TEMPS_START 0x380 // 0x380-0x3FF

#define HVC_VCU_AMS_IMD 0x001

// Interface

typedef struct CanRx {
    bool isRecent;
    uint8_t dlc;
    uint8_t data[8];
} CanRx;

/**
 * Tell the CAN driver to copy all incoming packets with a given ID to the given mailbox address.
 * Mailbox must be pre-defined
 * @param id ID of the CAN packets you want to add
 * @param mask Mask of the CAN packets you want to add
 * @param mailbox Pointer to where the incoming packet is stored.
 */
void can_addMailbox(uint16_t id, uint16_t mask, CanRx* mailbox);

/**
 * Get the mailbox associated with the ID
 * @param id ID of the CAN packet.
 * @return Pointer to the associated mailbox, returns null if it doesn't exist
 */
CanRx* can_getMailbox(uint16_t id);

/**
 * Update the corresponding mailboxes, emptying the RxFifo.
 */
void can_processRxFifo(uint8_t* RxData, uint16_t id, uint8_t dlc, uint32_t fault);

/**
 * Clear the recency of the mailboxes.
 */
void can_clearMailboxes();

/**
 * Put a CAN packet in the Tx FIFO.
 */
void can_send(uint16_t id, uint8_t dlc, uint8_t data[8]);

/**
 * Read a parameter from the packet, specifying which bytes to read.
 * This assumes that the packets are little endian, which is the standard set in CM200dz
 */
uint64_t can_read_packet(const uint8_t data[8], uint8_t start_byte, uint8_t end_byte);

/**
 * Write a parameter to the packet, specifying which bytes to write.
 * This assumes that the packets are little endian, which is the standard set in CM200dz
 */
void can_write_packet(uint8_t data[8], uint8_t start_byte, uint8_t end_byte, uint64_t value);

#endif //LONGHORN_LIBRARY_2024_CAN_H
