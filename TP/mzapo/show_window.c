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
#include <string.h>

c_set **generated_sets; //stored c in structure

bool draw_set_with_parameters(int set, int depth);

bool check_end();

void show_window() {
    stop_show_window = 0;
    int i, j;
    bool break_flag = false;
    generated_sets = get_c_list();
    while (1) {
        int iter = 2;
        for (i = 0; i < NUMBER_OF_STORED_C; i++) {
            for (j = 1; j < 400; j += iter) {

                if (!draw_set_with_parameters(i, j)) {
                    break_flag = true;
                    break;
                }

                if (j < 20) {
                    iter = 1;
                } else if (j < 100) {
                    iter = 10;
                } else if (j < 400) {
                    iter = 30;
                } else {
                    iter = 150;
                }

                if (check_end()) {
                    break_flag = true;
                    break;
                }
            }
            if (break_flag) break;
            sleep(2);
        }
        if (break_flag) break;
    }
    for (int i = 0; i < NUMBER_OF_STORED_C; i++) {
        free(*(generated_sets + i));
    }
    free(generated_sets);

}

bool check_end() {
    bool isclicked_blue = false, isclicked_red = false, isclicked_green = false;
    uint32_t rgb_knobs_value;

    rgb_knobs_value = *(volatile uint32_t *) (mem_base + SPILED_REG_KNOBS_8BIT_o); //get uint with value

    isclicked_red = (rgb_knobs_value & 0xFF000000) >> 24 == 4 ? true : false;
    isclicked_green = (rgb_knobs_value & 0xFF000000) >> 24 == 2 ? true : false;
    isclicked_blue = (rgb_knobs_value & 0xFF000000) >> 24 == 1 ? true : false;

    if (isclicked_red || isclicked_green || isclicked_blue || stop_show_window) {
        printf("Abort!\n");
        stop_show_window = 0;
        return true;
    }
    return false;
}

pthread_t drawing;

bool draw_set_with_parameters(int set, int depth) {

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

    int err = pthread_create(&drawing, NULL, draw_set, fractal);
    if (err != 0) {
        printf("ERROR occurred while creating new thread!\nReason: %s\nexiting...\n", strerror(err));
        return false;
    }
    pthread_join(drawing, NULL);
    return true;
}