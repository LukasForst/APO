/**
 * @author Lukas Forst
 * */

#include "display_writting.h"

#include "font_types.h"
#include "color.h"
#include "main.h"

#include <string.h>


uint16_t *write_string(char *string, uint16_t *data) {
    int row = 1;
    int column = 0;
    size_t size_of_string = strlen(string);
    for (int i = 0; i < size_of_string; i++) {
        if (*(string + i) == '\n' || column >= WIDTH) {
            row++;
            column = 0;
            continue;
        }

        data = put_char_there(data, *(string + i), row, column++);
    }
    return data;
}

uint16_t *put_char_there(uint16_t *data, char c, int row, int column) {
    for (int i = 0; i < 16; i++) {
        *(data + WIDTH * (i + row * 16) + column * 8 + 0) =
                font_rom8x16.bits[(int) c * 16 + i] >> 15 & 1 ? WHITE : BLACK;
        *(data + WIDTH * (i + row * 16) + column * 8 + 1) =
                font_rom8x16.bits[(int) c * 16 + i] >> 14 & 1 ? WHITE : BLACK;
        *(data + WIDTH * (i + row * 16) + column * 8 + 2) =
                font_rom8x16.bits[(int) c * 16 + i] >> 13 & 1 ? WHITE : BLACK;
        *(data + WIDTH * (i + row * 16) + column * 8 + 3) =
                font_rom8x16.bits[(int) c * 16 + i] >> 12 & 1 ? WHITE : BLACK;
        *(data + WIDTH * (i + row * 16) + column * 8 + 4) =
                font_rom8x16.bits[(int) c * 16 + i] >> 11 & 1 ? WHITE : BLACK;
        *(data + WIDTH * (i + row * 16) + column * 8 + 5) =
                font_rom8x16.bits[(int) c * 16 + i] >> 10 & 1 ? WHITE : BLACK;
        *(data + WIDTH * (i + row * 16) + column * 8 + 6) =
                font_rom8x16.bits[(int) c * 16 + i] >> 9 & 1 ? WHITE : BLACK;
        *(data + WIDTH * (i + row * 16) + column * 8 + 7) =
                font_rom8x16.bits[(int) c * 16 + i] >> 8 & 1 ? WHITE : BLACK;
    }
    return data;
}
