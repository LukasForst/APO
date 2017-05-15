/**
 * @author Lukas Forst
 * */

#ifndef APO_COLOR_H
#define APO_COLOR_H

#include <stdint.h>

#define DEFAULT_DEPTH 255

/**
 * Simple colors.
 * */
static const uint16_t RED = 0b1111100000000000;
static const uint16_t GREEN = 0b0000011111100000;
static const uint16_t BLUE = 0b0000000000011111;

/**
 * Structure holding generated color, RGB represented by uint8_t
 * */
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color;

/**
 * Convert given numbers to the one uint16_t color (RGB565)
 * */
uint16_t convert(uint8_t red, uint8_t green, uint8_t blue);

/**
 * Convert whole structure color to the uint16_t color (RGB565)
 * */
uint16_t convert_struct(color *c);

#endif //APO_COLOR_H
