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
    fread(old_ppm,size,1,f);

    int tmp;

    FILE *newf = fopen(output, "wb");
    fprintf(newf,"P6\n%d\n%d\n%d\n",picture_width, height, max_colour);

    register uint32_t idx = 0, w = 1;
    for(; idx != size;){

        if (w == picture_width || w == 1 || idx < width || idx >= (width*height-width)) {
            fputc(old_ppm[idx], f);
            fputc(old_ppm[idx+1], f);
            fputc(old_ppm[idx+2], f);

            if (w == picture_width)
                w = 1;
            else ++w;
            idx += 3;
        } else {
            for(int i = 0; i < 3; idx++, i++){
                tmp = 5*old_ppm[idx];
                tmp -= old_ppm[idx - 3];
                tmp -= old_ppm[idx + 3];
                tmp -= old_ppm[idx - width];
                tmp -= old_ppm[idx + width];
                tmp = tmp > 255 ? 255 : tmp;
                tmp = tmp < 0 ? 0 : tmp;
                fputc(tmp, newf);
            }
            w++;
        }

    }

    fclose(newf);
    fclose(f);
    return EXIT_SUCCESS;
}