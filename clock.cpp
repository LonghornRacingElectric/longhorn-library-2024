#include "clock.h"

static uint32_t lastTimeRecorded = 0;

void clock_init() {
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SysTick_IRQn);
}

float clock_getDeltaTime() {
    uint32_t time = HAL_GetTick();
    float deltaTime = ((float)(time - lastTimeRecorded)) / 1000000.0f;
    lastTimeRecorded = time;
    return deltaTime;
}
