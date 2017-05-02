//
// Created by lukas on 5/1/17.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "color.h"
#include "sets.h"

int main() {
    int width = 800, height = 600;
    double c_real = -0.7, c_imag = 0.27015, move_x = 0, move_y = 0;

    FILE *fp = fopen("pic.ppm", "wb");
    fprintf(fp, "P6\n%d\n%d\n255\n", width, height);

    //color **j = generate_julia(width, height, move_x, move_y, c_real, c_imag, 400);
    color ** j = generate_mandelbrot(width, height, move_x, move_y, 50);

    for (int i = 0; i < width * height; i++) {
        fwrite(*(j + i), sizeof(color), 1, fp);
        free(*(j + i));
    }
    free(j);

    fclose(fp);
}