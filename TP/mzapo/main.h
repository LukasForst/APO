/**
 * @author Lukas Forst
 * */

#ifndef TP_MAIN_H
#define TP_MAIN_H

#define WIDTH 480
#define HEIGHT 320

unsigned char *parlcd_mem_base;

typedef struct {
    double c_real;
    double c_imaginary;
    double x;
    double y;
    uint32_t depth;
    uint32_t set_number;
} parameters_t;

/**
 * Active parameters of the image.
 * */
parameters_t parameters;

/**
 * Display given set in the device. Function is running in the new thread.
 * */
void *draw_set(void *args);


/**
 * Put char in the field with data.
 * */
uint16_t *put_char_there(uint16_t *data, char c, int row, int column);

/**
 * Write whole string to the display.
 * */
uint16_t *write_string(size_t size_of_string, char *string, uint16_t *data, int row);

#endif //TP_MAIN_H
