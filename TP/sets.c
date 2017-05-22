/**
 * @author Lukas Forst
 * */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include "color.h"
#include "sets.h"

#define DEFAULT_ITERATIONS 400

uint16_t *
generate_julia(int width, int height, double move_x, double move_y, double c_real, double c_imag, int max_iterations) {
    max_iterations = max_iterations == 0 ? DEFAULT_ITERATIONS : max_iterations;

    double new_real, new_imag, old_real, old_imag;   //real and imaginary parts of new and old z

    uint16_t *result = (uint16_t *) calloc(width * height, sizeof(uint16_t)); // memory for the result
    unsigned int result_idx = 0;

    //some constants that are computed before whole loop
    const double real_part_const = 3.0 / width;
    const double real_part_add = 3.0 / 2.0;
    const double imag_part_const = 2.0 / height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            new_real = real_part_const * x - real_part_add + move_x;
            new_imag = imag_part_const * y - 1 + move_y;

            int i;
            for (i = 0; i < max_iterations; i++) {
                old_real = new_real;
                old_imag = new_imag;

                new_real = old_real * old_real - old_imag * old_imag + c_real;
                new_imag = 2 * old_real * old_imag + c_imag;

                // break when number doesn't belongs to the set
                if (fabs(new_real * new_real + new_imag * new_imag) > 2) break;
            }

            // let's give it some color
            double t = (double) i / (double) max_iterations;
            uint8_t red = (uint8_t) (9 * (1 - t) * t * t * t * DEFAULT_DEPTH);
            uint8_t green = (uint8_t) (15 * (1 - t) * (1 - t) * t * t * DEFAULT_DEPTH);
            uint8_t blue = (uint8_t) (8.5 * (1 - t) * (1 - t) * (1 - t) * t * DEFAULT_DEPTH);

            *(result + result_idx++) = convert(red, green, blue);
        }
    }
    return result;
}

c_set **get_c_list() {
    uint32_t number_of_elements = NUMBER_OF_STORED_C, idx_c_list = 0;

    c_set **c_list = (c_set **) calloc(number_of_elements, sizeof(c_set *));
    for (int i = 0; i < number_of_elements; i++) {
        c_set *c = (c_set *) calloc(1, sizeof(c_set));
        *(c_list + i) = c;
    }

    (*(c_list + idx_c_list))->real = -0.4;
    (*(c_list + idx_c_list++))->imaginary = 0.6;

    (*(c_list + idx_c_list))->real = 0.285;
    (*(c_list + idx_c_list++))->imaginary = 0;

    (*(c_list + idx_c_list))->real = 0.285;
    (*(c_list + idx_c_list++))->imaginary = 0.01;

    (*(c_list + idx_c_list))->real = 0.45;
    (*(c_list + idx_c_list++))->imaginary = 0.1428;

    (*(c_list + idx_c_list))->real = -0.70176;
    (*(c_list + idx_c_list++))->imaginary = -0.3842;

    (*(c_list + idx_c_list))->real = -0.835;
    (*(c_list + idx_c_list++))->imaginary = 0.2321;

    (*(c_list + idx_c_list))->real = -0.8;
    (*(c_list + idx_c_list++))->imaginary = 0.156;

    (*(c_list + idx_c_list))->real = -0.7269;
    (*(c_list + idx_c_list++))->imaginary = 0.1889;

    (*(c_list + idx_c_list))->real = 0;
    (*(c_list + idx_c_list++))->imaginary = -0.8;

    (*(c_list + idx_c_list))->real = 1 - 1.618033988;
    (*(c_list + idx_c_list++))->imaginary = 0;

    (*(c_list + idx_c_list))->real = -1.037;
    (*(c_list + idx_c_list++))->imaginary = 0.17;

    (*(c_list + idx_c_list))->real = -0.52;
    (*(c_list + idx_c_list++))->imaginary = 0.57;

    (*(c_list + idx_c_list))->real = -0.624;
    (*(c_list + idx_c_list++))->imaginary = 0.435;

    (*(c_list + idx_c_list))->real = -0.7;
    (*(c_list + idx_c_list++))->imaginary = 0.27015;

    return c_list;
}