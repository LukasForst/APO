/**
 * @author Lukas Forst
 * */

#ifndef TP_DISPLAY_WRITTING_H
#define TP_DISPLAY_WRITTING_H

#include <stdint.h>

/**
 * Write whole string to the display.
 * */
uint16_t *write_string(char *string, uint16_t *data);

/**
 * Put char in the field with data.
 * */
uint16_t *put_char_there(uint16_t *data, char c, int row, int column);


#endif //TP_DISPLAY_WRITTING_H
