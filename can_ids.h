//
// Created by yashk on 10/22/2023.
//

#ifndef LONGHORN_LIBRARY_2024_CAN_IDS_H
#define LONGHORN_LIBRARY_2024_CAN_IDS_H

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

#endif //LONGHORN_LIBRARY_2024_CAN_IDS_H
