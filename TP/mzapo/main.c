#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include "sets.h"
#include "color.h"

#define WIDTH 480
#define HEIGHT 320

unsigned char *parlcd_mem_base;

void *draw_set(void *args);

int main(int argc, char *argv[]) {
    unsigned char *mem_base;
    int i, j;

    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0); //display
    mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0); // buttons

    if (parlcd_mem_base == NULL || mem_base == NULL) {
        printf("Memory error!\n");
        exit(1);
    }
    parlcd_hx8357_init(parlcd_mem_base);

    double c_real = -0.7, c_imag = 0.27015, move_x = 0.0, move_y = 0.0; //-3,3 is max for x, -2,2 is max for y
    color **fractal;
    c_set **generated_sets = get_c_list(); //stored c in structure

    //fractal = generate_julia(WIDTH, HEIGHT, move_x, move_y, c_real, c_imag, 400);
    //fractal = generate_mandelbrot(WIDTH, HEIGHT, move_x, move_y, 200);

/*
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (i = 0; i < WIDTH * HEIGHT; i++) {
        color *c = *(fractal + i);
        uint16_t final_color = convert_struct(c);
        parlcd_write_data(parlcd_mem_base, final_color);
        free(*(fractal +i));
    }
    free(fractal);
*/

    uint32_t rgb_knobs_value;
    unsigned int uint_val;

    int depth = 500;

    double const x_path = (double) 6 / (double) 255;
    double const y_path = (double) 4 / (double) 255;

    double last_x = 0, last_y = 0;
    uint32_t x_value = 0, y_value = 0, c_sets = 0, last_c_sets = 0;

    pthread_t drawing; //drawing thread
    while (1) {
        rgb_knobs_value = *(volatile uint32_t *) (mem_base + SPILED_REG_KNOBS_8BIT_o); //get uint with value

        x_value = (rgb_knobs_value & 0x00FF0000) >> 16; // red button value
        move_x = x_value * x_path - 3;

        y_value = (rgb_knobs_value & 0x0000FF00) >> 8; // green button value
        move_y = y_value * y_path - 2;

        c_sets = (rgb_knobs_value & 0x000000FF) % NUMBER_OF_STORED_C; // blue button value

        printf("x = %f, y = %f, c set = %d\n", move_x, move_y, c_sets);

        if (last_x != move_x || last_y != move_y || last_c_sets != c_sets) {
            c_set *c = *(generated_sets + c_sets);
            c_real = c->real;
            c_imag = c->imaginary;

            fractal = generate_julia(WIDTH, HEIGHT, move_x, move_y, c_real, c_imag, depth);
            //draw_set(fractal, parlcd_mem_base);

            int err = pthread_create(&drawing, NULL, draw_set, fractal);
            if (err) {
                exit(EXIT_FAILURE);
            }

            last_x = move_x;
            last_y = move_y;
            last_c_sets = c_sets;
        }


        struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 5 * 1000 * 1000};
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }
}

void *draw_set(void *args) {
    color **fractal;
    fractal = (color **) args;

    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        color *c = *(fractal + i);
        uint16_t final_color = convert_struct(c);
        parlcd_write_data(parlcd_mem_base, final_color);
        free(*(fractal + i));
    }
    free(fractal);
    return NULL;
}