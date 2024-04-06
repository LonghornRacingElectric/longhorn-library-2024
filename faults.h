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

/*
 * First 8 bits relate to the VCU core to see if that's causing the fault
 */
#define FAULT_VCU_CORE_APPS (1 << 0)
#define FAULT_VCU_CORE_BSE (1 << 1)

/*
 * Next 8 bits relate to the VCU board peripherals in general. This includes:
 * CAN, SPI, GPS, NVM, CELL, ADC
 */
#define FAULT_VCU_CAN (1 << 8) // CAN will not be working if this is set
#define FAULT_VCU_SPI (1 << 9) // SPI will not be working if this is set
#define FAULT_VCU_GPS (1 << 10) // GPS will not be working if this is set
#define FAULT_VCU_NVM (1 << 11)
#define FAULT_VCU_CELL (1 << 12)
#define FAULT_VCU_ADC (1 << 13)

/**
 * Next 8 bits relate to on-board communication and their status
 */
#define FAULT_VCU_GPS_BAD_RX (1 << 16)
#define FAULT_VCU_GPS_BAD_TX (1 << 17)
#define FAULT_VCU_CAN_BAD_TX (1 << 18)
#define FAULT_VCU_CAN_BAD_RX (1 << 19)



#endif //LONGHORN_LIBRARY_2024_FAULTS_H
