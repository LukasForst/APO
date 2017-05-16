/**
 *@author Lukas Forst
 * */

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

/**
 * Check state of the buttons every second.
 * */
void *check_end(void *args);

void show_window() {
    stop_show_window = false;

    int i, j, iter = 2;
    bool break_flag = false;

    pthread_t buttons_check, drawing;
    int err = pthread_create(&buttons_check, NULL, check_end, NULL); //start thread which will be checking buttons
    if (err != 0) {
        printf("ERROR occurred while creating new thread!\nReason: %s\nexiting...\n", strerror(err));
        return;
    }

    c_set **generated_sets = get_c_list(); //generate parameters for the julia sets
    uint16_t *fractal;
    c_set *c;

    while (true) {
        for (i = 0; i < NUMBER_OF_STORED_C; i++) {
            for (j = 1; j < 400; j += iter) {
                c = *(generated_sets + i);

                fractal = generate_julia(WIDTH, HEIGHT, 0, 0, c->real, c->imaginary, j);
                printf("x = %f, y = %f, c set = %d, depth = %d\n", 0.0, 0.0, i, j);

                //set parameters of the image
                parameters.c_real = c->real;
                parameters.c_imaginary = c->imaginary;
                parameters.depth = j;
                parameters.set_number = i;
                parameters.x = 0;
                parameters.y = 0;

                int err = pthread_create(&drawing, NULL, draw_set, fractal);
                if (err != 0) {
                    printf("ERROR occurred while creating new thread!\nReason: %s\nexiting...\n", strerror(err));
                    break_flag = true;
                    break;
                }

                iter = iter / 10; // faster grow of the depth

                pthread_join(drawing, NULL); //wait for previous thread
                if (stop_show_window) {
                    break_flag = true;
                    break;
                }
            }
            if (break_flag) break;
            sleep(2); //wait 2 secs before showing another fractal
        }
        if (break_flag) break;
    }
    //cleanup
    pthread_join(drawing, NULL);
    pthread_join(buttons_check, NULL);

    for (int i = 0; i < NUMBER_OF_STORED_C; i++) {
        free(*(generated_sets + i));
    }
    free(generated_sets);

    stop_show_window = false;
    return;
}


void *check_end(void *args) {
    bool isclicked_blue = false, isclicked_red = false, isclicked_green = false;
    uint32_t rgb_knobs_value;

    while (true) {
        rgb_knobs_value = *(volatile uint32_t *) (mem_base + SPILED_REG_KNOBS_8BIT_o); //get uint with value

        isclicked_red = (rgb_knobs_value & 0xFF000000) >> 24 == 4 ? true : false;
        isclicked_green = (rgb_knobs_value & 0xFF000000) >> 24 == 2 ? true : false;
        isclicked_blue = (rgb_knobs_value & 0xFF000000) >> 24 == 1 ? true : false;

        if (isclicked_red || isclicked_green || isclicked_blue || stop_show_window) {
            printf("Abort!\n");
            stop_show_window = true;
            break;
        }
        sleep(1);
    }
    return NULL;
}