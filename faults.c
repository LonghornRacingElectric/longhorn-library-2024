#include "faults.h"

uint32_t vcu_fault_vector = 0;
uint32_t inv_fault_vector = 0;
uint32_t pdu_fault_vector = 0;
uint32_t hvc_fault_vector = 0;
uint32_t dash_fault_vector = 0;

void fault_set(uint32_t* fault_vector, uint32_t fault) {
  *fault_vector |= fault;
}

void clear_fault(uint32_t* fault_vector, uint32_t fault) {
  *fault_vector &= ~fault;
}

void clear_all_faults(uint32_t* fault_vector) {
  *fault_vector = 0;
}

bool check_fault(const uint32_t* fault_vector, uint32_t fault) {
  return (*fault_vector & fault) != 0;
}
