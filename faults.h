#ifndef LONGHORN_LIBRARY_2024_FAULTS_H
#define LONGHORN_LIBRARY_2024_FAULTS_H

#include <stdint.h>
#include <stdbool.h>
extern uint32_t vcu_fault_vector;
extern uint32_t inv_fault_vector;
extern uint32_t pdu_fault_vector;
extern uint32_t hvc_fault_vector;
extern uint32_t dash_fault_vector;

/**
 * Set a fault bit in the fault vector.
 * @param fault_vector
 * @param fault
 */
void fault_set(uint32_t* fault_vector, uint32_t fault);

/**
 * Clear a fault bit in the fault vector.
 * @param fault_vector
 * @param fault
 */
void fault_clear(uint32_t* fault_vector, uint32_t fault);

/**
 * Clear all fault bits in the fault vector.
 * @param fault_vector
 */
void fault_clearAll(uint32_t* fault_vector);

/**
 * Check if a fault bit is set in the fault vector.
 * @param fault_vector
 * @param fault
 * @return true if fault is set
 */
bool fault_check(const uint32_t* fault_vector, uint32_t fault);

// VCU FAULTS
/**
 * These are core faults, which are determined by VcuCore. See Libs/VcuCore/src/VcuModel.h
 */
#define FAULT_VCU_APPS 0x00000001
#define FAULT_VCU_BSE 0x00000002
#define FAULT_VCU_STOMPP 0x00000004
#define FAULT_VCU_STEER 0x00000008
#define FAULT_VCU_TORQUE_MAP 0x00000010
#define FAULT_VCU_TRACTION_CTRL 0x00000020
#define FAULT_VCU_TRACK_POSITION 0x00000040
/**
 * These are faults for when parameters are failed to be set.
 */
#define FAULT_VCU_INVPARAMS 0x00000100
#define FAULT_VCU_HVCPARAMS 0x00000200
#define FAULT_VCU_PDUPARAMS 0x00000400
#define FAULT_VCU_DASHPARAMS 0x00000800
/**
 * These are local communication faults, where data cannot be properly validated with this device / protocol. See schematic for VCU.
 * This includes receive / send errors, data corruption, and data timeouts.
 */
#define FAULT_VCU_EEPROM 0x00010000
#define FAULT_VCU_IMU 0x00020000
#define FAULT_VCU_NVM 0x00040000
#define FAULT_VCU_CELL 0x00080000
#define FAULT_VCU_BSPD 0x00100000
#define FAULT_VCU_CAN 0x00200000
#define FAULT_VCU_SPI 0x00400000
#define FAULT_VCU_UART 0x00800000
/**
 * These are external communication faults, where data cannot be properly validated with this enclosure / board through CAN. See car lmao.
 */
#define FAULT_VCU_INV 0x01000000
#define FAULT_VCU_HVC 0x02000000
#define FAULT_VCU_PDU 0x04000000
#define FAULT_VCU_DASH 0x08000000
#define FAULT_VCU_WHS_FRONT 0x10000000
#define FAULT_VCU_UPR_FRONT 0x20000000
#define FAULT_VCU_WHS_REAR 0x40000000
#define FAULT_VCU_UPR_REAR 0x80000000

#endif //LONGHORN_LIBRARY_2024_FAULTS_H
