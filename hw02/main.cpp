#include <iostream>
#include <string.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 *   0  -1  0
    -1  5   -1
     0	-1	0
 */

int main(int argc, char* argv[]) {
    char *fname;
    const char *output = "output.ppm";

    if(argc > 1){
        fname  = argv[1];
    } else {
        return EXIT_FAILURE;
    }

    FILE *f = fopen(fname, "rb");
    if(!f){
        fprintf(stderr, "WRONG FILE\n");
        return EXIT_FAILURE;
    }

    uint32_t height, width, max_colour, picture_width;
    fscanf (f, "P6 %d %d %d",&picture_width, &height, &max_colour);
    width = 3*picture_width;

    uint32_t size = width*height;
    char *old_ppm = new char[size];
    char *new_ppm = new char[size];
    fread(old_ppm,size,1,f);


    int row, column, colour, tmp;

    for(row = 0; row < height; row++){
        for(column = 0; column < width; column += 3){

            for(colour = 0; colour < 3; colour++){
                //new_ppm[width * row + column + colour] = (-1)*old_ppm[width * (row - 1) + column + colour];
                tmp = 0;
                tmp
                        += row > 0 ? (-1)*old_ppm[width * (row - 1) + column + colour] : 0;
                tmp
                        += column > 0 ? (-1)*old_ppm[width * row + (column - 1) + colour] : 0;
                tmp
                        += 5*old_ppm[width * row + column + colour];
                tmp
                        += column < width - 1 - colour ? (-1)*old_ppm[width * row + (column + 1) + colour] : 0;
                tmp
                        += row < height - 1 ? (-1)*old_ppm[width * (row + 1) + column + colour] : 0;
                tmp = tmp > 255 ? 255 : tmp;
                tmp = tmp < 0 ? 0 : tmp;
                new_ppm[width * row + column + colour] = (char) tmp;
            }
        }
    }


    //write ppm
    FILE *newf = fopen(output, "wb");
    fprintf(newf,"P6\n%d\n%d\n%d\n",picture_width, height, max_colour);
    fwrite(new_ppm, width, height, newf);

    fclose(newf);
    fclose(f);
    return EXIT_SUCCESS;
}