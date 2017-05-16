/**
 * @author Lukas Forst
 * */

#ifndef TP_MAIN_H
#define TP_MAIN_H

#define WIDTH 480
#define HEIGHT 320

#include <stdint.h>

/**
 * Access to the lcd from everywhere.
 * */
unsigned char *parlcd_mem_base;

/**
 * Access to the buttons from everywhere.
 * */
unsigned char *mem_base;

/**
 * Structure containing parameters of the displayed image in the device.
 * */
typedef struct {
    double c_real;
    double c_imaginary;
    double x;
    double y;
    uint32_t depth;
    uint32_t set_number;
} parameters_t;

/**
 * Active parameters of the image.
 * */
parameters_t parameters;

/**
 * Display given set in the device. Function is running in the new thread.
 * */
void *draw_set(void *args);

#endif //TP_MAIN_H
