/**
 * @author Lukas Forst
 * */

#ifndef TP_MAIN_H
#define TP_MAIN_H

#define WIDTH 480
#define HEIGHT 320

unsigned char *parlcd_mem_base;

/**
 * Display given set in the device. Function is running in the new thread.
 * */
void *draw_set(void *args);

#endif //TP_MAIN_H
