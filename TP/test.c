//
// Created by lukas on 5/1/17.
//
#include <stdio.h>
#include <stdint.h>
#include "color.h"

int main() {
    uint16_t res = convert(255, 0, 0);
    printf("%x\n", res);
}