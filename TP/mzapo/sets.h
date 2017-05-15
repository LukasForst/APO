/**
 * @author Lukas Forst
 * */

#ifndef APO_SETS_H
#define APO_SETS_H

#include "color.h"

#define NUMBER_OF_STORED_C 14

/**
 * Structure defining c number with real and imaginary part.
 * */
typedef struct {
    double real;
    double imaginary;
} c_set;

/**
 * Generate julia set according to the given parameters.
 * */
color **
generate_julia(int width, int height, double move_x, double move_y, double c_real, double c_imag, int max_iterations);

/**
 * Generate mandelbrot according to the given parameters.
 * */
color **generate_mandelbrot(int width, int height, double move_x, double move_y, int max_iterations);

/**
 * Get list of predefined real and imaginary parts of the c.
 * */
c_set **get_c_list();

#endif //APO_SETS_H
