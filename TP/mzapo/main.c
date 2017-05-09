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

int main(int argc, char *argv[]) {
    unsigned char *parlcd_mem_base;
    int i, j;

    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

    if (parlcd_mem_base == NULL)
        exit(1);

    parlcd_hx8357_init(parlcd_mem_base);
    //parlcd_hx8357_init(parlcd_mem_base);

    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            parlcd_write_data(parlcd_mem_base, 0x0000);
        }
    }

    double c_real = -0.7, c_imag = 0.27015, move_x = 0, move_y = 0; //-3,3 is max for x, -2,2 is max for y
    //color **fractal = generate_julia(WIDTH, HEIGHT, move_x, move_y, c_real, c_imag, 400);
    color **fractal = generate_mandelbrot(WIDTH, HEIGHT, move_x, move_y, 200);

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
    int depth = 10;
    while (1) {
        struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 200 * 1000 * 1000};
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);

        fractal = generate_julia(WIDTH, HEIGHT, move_x, move_y, c_real, c_imag, depth);
        depth = depth >= 2000 ? 10 : depth + 100;
        move_x = move_x >= 3 ? -3 : move_x + 0.1;

        parlcd_write_cmd(parlcd_mem_base, 0x2c);
        for (i = 0; i < WIDTH * HEIGHT; i++) {
            color *c = *(fractal + i);
            uint16_t final_color = convert_struct(c);
            parlcd_write_data(parlcd_mem_base, final_color);
            free(*(fractal + i));
        }
        free(fractal);
    }

    return 0;
}