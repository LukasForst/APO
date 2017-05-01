//
// Created by lukas on 5/1/17.
//

#include "color.h"

uint16_t convert(uint8_t red, uint8_t green, uint8_t blue) {
    uint16_t result =
            ((((uint16_t) red) << 8) & RED) | ((((uint16_t) green) << 3) & GREEN) | ((((uint16_t) blue >> 3)) & BLUE);
    return result;
}

uint16_t convert_struct(color *c) {
    uint16_t result = ((((uint16_t) c->red) << 8) & RED) | ((((uint16_t) c->green) << 3) & GREEN) |
                      ((((uint16_t) c->blue >> 3)) & BLUE);
    return result;
}