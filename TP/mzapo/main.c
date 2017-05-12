#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/socket.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include "sets.h"
#include "color.h"
#include "udp_server.h"
#include "main.h"

int main(int argc, char *argv[]) {
    uint32_t rgb_knobs_value;
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

    uint32_t depth = 500, last_depth = depth;

    double const x_range = (double) 6 / (double) 255;
    double const y_range = (double) 4 / (double) 255;
    double const depth_range = (double) 1000 / (double) 255;

    double last_x = 0, last_y = 0;
    uint32_t r_value = 0, g_value = 0, b_value = 0, c_sets = 0, last_c_sets = 0;
    bool isclicked_red = false, isclicked_green = false, isclicked_blue = false;

    pthread_t drawing; //drawing thread
    pthread_t udp;
    pthread_create(&udp, NULL, udp_listener, NULL); //let's start listening for the udp
    while (1) {
        rgb_knobs_value = *(volatile uint32_t *) (mem_base + SPILED_REG_KNOBS_8BIT_o); //get uint with value

        isclicked_red = (rgb_knobs_value & 0xFF000000) >> 24 == 4 ? true : false;
        isclicked_green = (rgb_knobs_value & 0xFF000000) >> 24 == 2 ? true : false;
        isclicked_blue = (rgb_knobs_value & 0xFF000000) >> 24 == 1 ? true : false;

        r_value = (rgb_knobs_value & 0x00FF0000) >> 16; // red button value

        g_value = (rgb_knobs_value & 0x0000FF00) >> 8; // green button value

        b_value = (rgb_knobs_value & 0x000000FF); // blue button value

        if (!isclicked_red && !isclicked_green && !isclicked_blue) {
            move_x = x_range * r_value - 3;
            move_y = y_range * g_value - 2;
            c_sets = b_value % NUMBER_OF_STORED_C;
        } else if (isclicked_red) {
            depth = b_value * depth_range;
        } else if (isclicked_green) {
            //do something
        } else if (isclicked_blue) {
            //do something
        }

        printf("x = %f, y = %f, c set = %d\n", move_x, move_y, c_sets);

        if (last_x != move_x || last_y != move_y || last_c_sets != c_sets || last_depth != depth) {
            c_set *c = *(generated_sets + c_sets);
            c_real = c->real;
            c_imag = c->imaginary;

            fractal = generate_julia(WIDTH, HEIGHT, move_x, move_y, c_real, c_imag, depth);

            if (pthread_create(&drawing, NULL, draw_set, fractal)) {
                printf("ERROR occurred while creating new thread!\n");
                break;
            }

            last_x = move_x;
            last_y = move_y;
            last_c_sets = c_sets;

            pthread_join(drawing, NULL); //wait for previous thread
        }
    }

    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        free(*(fractal + i));
    }
    free(fractal);


    for (int i = 0; i < NUMBER_OF_STORED_C; i++) {
        free(*(generated_sets + i));
    }
    free(generated_sets);

    pthread_cancel(udp);
    return 0;
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
    pthread_exit(NULL);
}

