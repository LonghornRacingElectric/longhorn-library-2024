#include "can.h"


void can_addMailbox(uint32_t id, canRx* mailbox);

void can_processRxFifo();

void can_send(uint32_t id, uint8_t dlc, uint8_t data[8]);
