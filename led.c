#include "led.h"
#include "tim.h"

void led_init() {
#ifndef STM32H7A3xxQ
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
#endif
}

static void led_setInt(uint8_t r, uint8_t g, uint8_t b) {
#ifndef STM32H7A3xxQ
    TIM2->CCR1 = b;
    TIM2->CCR2 = r;
    TIM2->CCR3 = g;
#endif
}

void led_set(float r, float g, float b) {
    led_setInt((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255));
}

void led_off() {
    led_setInt(0, 0, 0);
}

void led_rainbow(float deltaTime) {
    static uint8_t r = 255, g = 0, b = 0;
    static float timer = 0;

    timer += deltaTime * 3.0f * 255.0f;
    if(timer > 3.0f) {
        // one cycle every 1.5 seconds
        timer = 0.0f;
    } else {
        return;
    }

    if((r && g) || (!g && !b)) {
        r--;
        g++;
    } else if ((g && b) || (!b && !r)) {
        g--;
        b++;
    } else {
        b--;
        r++;
    }
    led_setInt(r/4, g/2, b/2);
}