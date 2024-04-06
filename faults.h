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
// void fault_set(uint32_t* fault_vector, uint32_t fault);
#define FAULT_SET(fault_vector, fault) (*(fault_vector) |= (fault))

/**
 * Clear a fault bit in the fault vector.
 * @param fault_vector
 * @param fault
 */
// void fault_clear(uint32_t* fault_vector, uint32_t fault);
#define FAULT_CLEAR(fault_vector, fault) (*(fault_vector) &= ~(fault))

/**
 * Clear all fault bits in the fault vector.
 * @param fault_vector
 */
// void fault_clearAll(uint32_t* fault_vector);
#define FAULT_CLEARALL(fault_vector) (*(fault_vector) = 0)

/**
 * Check if a fault bit is set in the fault vector.
 * @param fault_vector
 * @param fault
 * @return true if fault is set, false otherwise
 */
// bool fault_check(const uint32_t* fault_vector, uint32_t fault);
#define FAULT_CHECK(fault_vector, fault) ((*(fault_vector) & (fault)) != 0)

// VCU FAULTS
#define FAULT_VCU_CELLULAR_QUEUE_OVERFLOW 0x00000001
#define FAULT_VCU_INV_PARAMS 0x00000002


// HVC Faults
#define FAULT_HVC_CCS_HARDWARE 0x00000100
#define FAULT_HVC_CCS_OVERTEMP 0x00000200
#define FAULT_HVC_CCS_INCORRECT_VOLTAGE 0x00000400
#define FAULT_HVC_CCS_CHARGER_OFF 0x00000800

#define FAULT_HVC_CCS 0x01000000
#define FAULT_HVC_VCU 0x02000000
#define FAULT_HVC_PDU 0x04000000
#define FAULT_HVC_DSH 0x08000000

#endif //LONGHORN_LIBRARY_2024_FAULTS_H
