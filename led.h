#ifndef LONGHORN_LIBRARY_2024_LED_H
#define LONGHORN_LIBRARY_2024_LED_H

/**
 * Initialize LED.
 */
void led_init();

/**
 * Set debug LED to given RGB value (0-1 scale).
 * @param r red
 * @param g green
 * @param b blue
 */
void led_set(float r, float g, float b);

/**
 * Turn off debug LED.
 */
void led_off();

#endif //LONGHORN_LIBRARY_2024_LED_H
