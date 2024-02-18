#ifndef VCU_FIRMWARE_2024_ANGEL_CAN_IDS_H
#define VCU_FIRMWARE_2024_ANGEL_CAN_IDS_H

/**
 * This is the list of CAN IDs used by LHRe 2024 for Angelique.
 * CAN IDs are 11 bits long, so they range from 0 to 2047 (0x000 to 0x7FF).
 * The CAN messages are separated into three priorities, with medium and low having three subpriorities each.
 * Subpriorities are subjective to the user's discretion (i.e., you get to choose)
 * - High priority: 0x0A0 to 0x0FF (used for inverter communication and FSAE-required msgs, such as IMD and AMS)
 * - Medium priority: 0x100 to 0x2FF (used for commands and controls, along with error messages)
 * - Low priority: 0x300 to 0x4FF (used for pure telemetry, such as cell voltages and temperatures)
 * Furthermore, they are separated by device ID using bits 7-4, where the device is sending this msg.
 * The device IDs are as follows:
 * - 0x0: Reserved
 * - 0x1: Vehicle Control Unit (VCU)
 * - 0x2: High Voltage Control (HVC)
 * - 0x3: Power Distribution Unit (PDU)
 * - 0x4: Unsprung Module (UNS) note: there are four modules, so the location of the module is determined by bits 3-2
 * - 0x5: Dash Module (DSH)
 * - 0x6: High Voltage Control (HVC) Telemetry Messages
 * - 0x7: High Voltage Control (HVC) Telemetry Messages
 * - 0x8: High Voltage Control (HVC) Telemetry Messages
 * - 0x9: High Voltage Control (HVC) Telemetry Messages
 * - 0xA: Inverter (INV) note: the device only uses the high priority range so it is 0x0A0 to 0x0AF
 * - 0xB: Reserved
 * - 0xC: Inverter (INV) note: the device only uses the high priority range so it is 0x0C0 to 0x0CF
 * - 0xD: Open
 * - 0xE: Open
 * - 0xF: Open
 * The remaining bits are to specify the message ID. They are first come, first served. Some examples are below.
 * - 0x0AB: Inverter Fault Codes
 * - 0x112: VCU->PDU Cooling
 * - 0x370: HVC->VCU Cell Temperatures
 */

//Parameters to set to boards (unused currently)
#define VCU_HVC_PARAMS 0x010
#define VCU_PDU_PARAMS 0x011
#define VCU_UNS_PARAMS 0x012
#define VCU_DSH_PARAMS 0x013

/**
 * The following are the CAN IDs pre-defined by CM200DZ, the inverter we use.
 * All inverter messages are high priority, thus the ID structure is configured below.
 * [000 1010 ****] OR [000 1100 ****]
 */
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

/**
 * The following are the CAN IDs generated by the VCU.
 * Most are specifically non-telemetry messages (i.e., we need this message for something in the code).
 * [*** 0001 ****]
 */
#define VCU_PDU_BRAKELIGHT 0x110 //Sends out brake light info to PDU
#define VCU_PDU_BUZZER 0x111 //Sends out horn info to PDU
#define VCU_PDU_COOLING 0x112 //Sends out cooling info to PDU
#define VCU_HVC_COOLING 0x113 //Sends out cooling info to HVC
#define VCU_DASH_INFO1 0x710 //Sends info to be displayed on dash
#define VCU_DASH_INFO2 0x711 //Sends more info to be displayed on dash

/**
 * These are specifically telemetry messages from the VCU.
 * [011 0001 ****] OR [100 0001 ****]
 */

/**
 * The following are the CAN IDs generated by the HVC.
 * These are specifically non-telemetry messages (i.e., we need this message for something in the code)
 * [*** 0010 ****]
 */
#define HVC_VCU_AMS_IMD 0x020 //Stores AMS and IMD status
#define HVC_VCU_PACK_STATUS 0x220 //Stores pack voltage, current, and SOC
#define HVC_VCU_IMU_ACCEL 0x221 // Stores accel data of imu
#define HVC_VCU_IMU_GYRO 0x222 // Stores gyro data of imu
#define HVC_VCU_CCS_INFO 0x223 //Stores CCS output voltage and current
#define HVC_VCU_FAN_RPM 0x224 // Stores battery fan RPMs
#define HVC_DSH_FAULT_MSG 0x22F // Stores fault data of hvc

/**
 * These are specifically telemetry messages from the HVC.
 * Moreover, because there are so many messages, HVC has more than one range of IDs.
 */

#define HVC_VCU_CELL_VOLTAGES_START 0x370
#define HVC_VCU_CELL_VOLTAGES_END 0x39F // 48 assigned packets for voltages
#define HVC_VCU_CONTACTOR_STATUS 0x420 //Stores contactor status and general status of HVC
#define HVC_VCU_CELL_TEMPS_START 0x470 //
#define HVC_VCU_CELL_TEMPS_END 0x47F // 16 assigned packets for temps

/**
 * The following are pre-specified CAN-IDs that are used by the TC Charger for .. well .. charging the battery.
 * They are sent on a separate CAN bus, so will not collide with any other messages, and they also use the extended ID nomenclature
 */
#define HVC_CCS_LIMIT 0x1806E5F4 // Sends out the voltage and current limits to the charger
#define CCS_HVC_OUTPUT 0x18FF50E5 // Sends out at what voltage and current the charger is currently operating at


/**
 * The following are the CAN IDs generated by the PDU.
 * These are specifically non-telemetry messages (i.e., we need this message for something in the code)
 * [*** 0011 ****]
 */
#define PDU_VCU_THERMAL 0x230 //Stores cooling info
#define PDU_VCU_IMU_ACCEL 0x231 // Stores accel data of imu
#define PDU_VCU_IMU_GYRO 0x232 // Stores gyro data of imu
#define PDU_DSH_FAULT_MSG 0x23F //Stores fault info of pdu

/**
 * These are specifically telemetry messages from the PDU.
 * [011 0011 ****] OR [100 0011 ****]
 */
#define PDU_VCU_LVBAT 0x330 //Stores BMS info
#define PDU_VCU_LV_CURRENTS 0x331 //Stores current info
#define PDU_VCU_STATUS 0x332 //Stores Status of PDU


/**
 * The following are the CAN IDs generated by the UNS.
 * There are four unsprung enclosures, and their IDs are shown below
 * - UNS1: [*** 0100 00**] -> Front Right Sensor
 * - UNS2: [*** 0100 01**] -> Front Left Sensor
 * - UNS3: [*** 0100 10**] -> Rear Right Sensor
 * - UNS4: [*** 0100 11**] -> Rear Left Sensor
 */

#define UNS_VCU_DISP_1 0x340 //Stores displacement and time of wheel 1
#define UNS_VCU_IMU_1 0x341 //Stores speed and time of wheel 1
#define UNS_DSH_FAULT_MSG_1 0x343 //Stores fault info of whs
#define UNS_VCU_DISP_2 0x344 //Stores displacement and time of wheel 2
#define UNS_VCU_IMU_2 0x345 //Stores speed and time of wheel 2
#define UNS_DSH_FAULT_MSG_2 0x347 //Stores fault info of whs
#define UNS_VCU_DISP_3 0x348 //Stores displacement and time of wheel 3
#define UNS_VCU_IMU_3 0x349 //Stores speed and time of wheel 3
#define UNS_DSH_FAULT_MSG_3 0x34B //Stores fault info of whs
#define UNS_VCU_DISP_4 0x34C //Stores displacement and time of wheel 4
#define UNS_VCU_IMU_4 0x34D //Stores speed and time of wheel 4
#define UNS_DSH_FAULT_MSG_4 0x34F //Stores fault info of whs

/**
 * The following are the CAN IDs generated by the DSH.
 * [*** 0101 ****]
 */

#define DSH_FAULT_MSG 0x26F //Stores fault info of dsh

#define DSH_VCU_STATUS 0x360 //Stores status of dsh


// Interface
#endif //VCU_FIRMWARE_2024_ANGEL_CAN_IDS_H
