#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include "color.h"
#include "sets.h"

#define DEFAULT_ITERATIONS 100

color **
generate_julia(int width, int height, double move_x, double move_y, double c_real, double c_imag, int max_iterations) {
    max_iterations = max_iterations == 0 ? DEFAULT_ITERATIONS : max_iterations;

    double new_real, new_imag, old_real, old_imag;   //real and imaginary parts of new and old z

    color **result = (color **) calloc(width * height, sizeof(color **));
    unsigned int result_idx = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            new_real = 1.5 * (x - width / 2) / (0.5 * width) + move_x;
            new_imag = (y - height / 2) / (0.5 * height) + move_y;

            int i; // number of iterations
            for (i = 0; i < max_iterations; i++) {
                old_real = new_real;
                old_imag = new_imag;

                new_real = old_real * old_real - old_imag * old_imag + c_real;
                new_imag = 2 * old_real * old_imag + c_imag;

                if (fabs(new_real * new_real + new_imag * new_imag) > 2) break;

            }
            color *c = (color *) calloc(1, sizeof(color));

            double t = (double) i / (double) max_iterations;

            c->red = (uint8_t) (9 * (1 - t) * t * t * t * DEFAULT_DEPTH);;
            c->green = (uint8_t) (15 * (1 - t) * (1 - t) * t * t * DEFAULT_DEPTH);
            c->blue = (uint8_t) (8.5 * (1 - t) * (1 - t) * (1 - t) * t * DEFAULT_DEPTH);

            *(result + result_idx++) = c;
        }
    }
    return result;
}

color **generate_mandelbrot(int width, int height, double move_x, double move_y, int max_iterations) {
    max_iterations = max_iterations == 0 ? DEFAULT_ITERATIONS : max_iterations;

    //double zoom = 0.8; (0.5 * zoom * height)

    double pixel_imag, pixel_real;
    double new_real, new_imag, old_real, old_imag;   //real and imaginary parts of new and old z

    color **result = (color **) calloc(width * height, sizeof(color **));
    unsigned int result_idx = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixel_real = 1.5 * (x - width / 2) / (0.5 * width) + move_x;
            pixel_imag = (y - height / 2) / (0.5 * height) + move_y;
            new_real = new_imag = 0;

            int i; // number of iterations
            for (i = 0; i < max_iterations; i++) {
                old_real = new_real;
                old_imag = new_imag;

                new_real = old_real * old_real - old_imag * old_imag + pixel_real;
                new_imag = 2 * old_real * old_imag + pixel_imag;

                if (fabs(new_real * new_real + new_imag * new_imag) > 2) break;
            }
            color *c = (color *) calloc(1, sizeof(color));

            double t = (double) i / (double) max_iterations;
            c->red = (uint8_t) (9 * (1 - t) * t * t * t * DEFAULT_DEPTH);;
            c->green = (uint8_t) (15 * (1 - t) * (1 - t) * t * t * DEFAULT_DEPTH);
            c->blue = (uint8_t) (8.5 * (1 - t) * (1 - t) * (1 - t) * t * DEFAULT_DEPTH);

            /*
             * Another possible way of coloring picture.*/
            /*
            if(i == max_iterations){
                c->red = 0;
                c->green = 0;
                c ->blue = 0;
            } else {
                double z = sqrt(new_real * new_real + new_imag * new_imag);
                uint8_t brightness = 256. * log2(1.75 + i - log2(log2(z))) / log2(max_iterations);

                c->red = brightness;
                c->green = brightness;
                c ->blue = 255;
            }
            */

            *(result + result_idx++) = c;
        }
    }
    return result;
}