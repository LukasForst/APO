//
// Created by lukas on 5/1/17.
//

#ifndef APO_SETS_H
#define APO_SETS_H

#include "color.h"

#define NUMBER_OF_STORED_C 14

typedef struct {
    double real;
    double imaginary;
} c_set;

color **
generate_julia(int width, int height, double move_x, double move_y, double c_real, double c_imag, int max_iterations);

color **generate_mandelbrot(int width, int height, double move_x, double move_y, int max_iterations);

c_set **get_c_list();

#endif //APO_SETS_H
