#ifndef LONGHORN_LIBRARY_2024_CAN_H
#define LONGHORN_LIBRARY_2024_CAN_H

#include <stdint.h>

// CAN IDs

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
#define VCU_INV_PARAMETER_RW 0x0C1 //Sets inverter parameter r/w
#define INV_VCU_RESPONSE_RW 0x0C2 //Responds back success of parameter r/w


// Interface

typedef struct CanRx {
    bool isRecent;
    uint8_t dlc;
    uint8_t data[8];
} CanRx;

/**
 * Tell the CAN driver to copy all incoming packets with a given ID to the given mailbox address.
 * @param id ID of the CAN packets.
 * @param mailbox Pointer to where the incoming packet is stored.
 */
void can_addMailbox(uint32_t id, CanRx* mailbox);

/**
 * Empty the Rx FIFO and update the corresponding mailboxes.
 */
void can_processRxFifo();

/**
 * Put a CAN packet in the Tx FIFO.
 */
void can_send(uint32_t id, uint8_t dlc, uint8_t data[8]);

#endif //LONGHORN_LIBRARY_2024_CAN_H
