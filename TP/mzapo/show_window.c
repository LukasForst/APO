//
// Created by lukas on 5/16/17.
//

#include "show_window.h"

#include "main.h"
#include "sets.h"
#include "color.h"

#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

c_set **generated_sets; //stored c in structure

void draw_set_with_parameters(int set, int depth);

void show_window() {
    int i, j;
    uint32_t rgb_knobs_value;
    bool isclicked_blue = false, isclicked_red = false, isclicked_green = false;

    generated_sets = get_c_list();
    while (1) {
        rgb_knobs_value = *(volatile uint32_t *) (mem_base + SPILED_REG_KNOBS_8BIT_o); //get uint with value

        isclicked_red = (rgb_knobs_value & 0xFF000000) >> 24 == 4 ? true : false;
        isclicked_green = (rgb_knobs_value & 0xFF000000) >> 24 == 2 ? true : false;
        isclicked_blue = (rgb_knobs_value & 0xFF000000) >> 24 == 1 ? true : false;

        if (isclicked_red || isclicked_green || isclicked_blue) {
            sleep(1);
            return;
        }

        int iter = 2;
        for (i = 0; i < NUMBER_OF_STORED_C; i++) {
            for (j = 1; j < 400; j += iter) {

                draw_set_with_parameters(i, j);

                if (j < 20) {
                    iter = 2;
                } else if (j < 100) {
                    iter = 20;
                } else if (j < 400) {
                    iter = 100;
                } else {
                    iter = 150;
                }
            }
            sleep(30);
        }
    }
}

void draw_set_with_parameters(int set, int depth) {

    c_set *c = *(generated_sets + set);
    double c_real = c->real;
    double c_imag = c->imaginary;

    color **fractal = generate_julia(WIDTH, HEIGHT, 0, 0, c_real, c_imag, depth);
    printf("x = %f, y = %f, c set = %d, depth = %d\n", 0.0, 0.0, set, depth);

    //set parameters of the image
    parameters.c_real = c_real;
    parameters.c_imaginary = c_imag;
    parameters.depth = depth;
    parameters.set_number = set;
    parameters.x = 0;
    parameters.y = 0;

    pthread_t drawing;
    if (pthread_create(&drawing, NULL, draw_set, fractal)) {
        printf("ERROR occurred while creating new thread!\nexiting...\n");
    }

}