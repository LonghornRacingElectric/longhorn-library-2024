#include "led.h"
#include "tim.h"

static void led_setInt(uint8_t r, uint8_t g, uint8_t b) {
    TIM2->CCR1 = r;
    TIM2->CCR2 = g;
    TIM2->CCR3 = b;
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
    if(timer > 1.5f) {
        // one cycle every 1.5 seconds
        timer -= 1.5f;
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
    led_setInt(r, g, b);
}