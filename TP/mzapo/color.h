/**
 * @author Lukas Forst
 * */

#ifndef APO_COLOR_H
#define APO_COLOR_H

#define DEFAULT_DEPTH 255

#include <stdint.h>

/**
 * Simple colors.
 * */
static const uint16_t RED = 0b1111100000000000;
static const uint16_t GREEN = 0b0000011111100000;
static const uint16_t BLUE = 0b0000000000011111;
static const uint16_t BLACK = 0b0000000000000000;
static const uint16_t WHITE = 0b1111111111111111;

/**
 * Convert given numbers to the one uint16_t color (RGB565)
 * */
uint16_t convert(uint8_t red, uint8_t green, uint8_t blue);

#endif //APO_COLOR_H
