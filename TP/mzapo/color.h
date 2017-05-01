//
// Created by lukas on 5/1/17.
//

#ifndef APO_COLOR_H
#define APO_COLOR_H

#include <stdint.h>

#define DEFAULT_DEPTH 255

static const uint16_t RED = 0b1111100000000000;
static const uint16_t GREEN = 0b0000011111100000;
static const uint16_t BLUE = 0b0000000000011111;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color;

uint16_t convert(uint8_t red, uint8_t green, uint8_t blue);

uint16_t convert_struct(color *c);

#endif //APO_COLOR_H
