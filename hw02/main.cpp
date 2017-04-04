#include <iostream>
#include <string.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

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

    int height, width, max_colour;
    fscanf (f, "P6 %d %d %d",&width, &height, &max_colour);

    char *data = new char[width*height * 3];
    fread(data,width*height * 3,1,f);



    //write ppm
    FILE *newf = fopen(output, "wb");
    fprintf(newf,"P6\n%d\n%d\n%d",width, height, max_colour);
    fwrite(data, width * 3, height, newf);

    fclose(newf);
    fclose(f);
    return EXIT_SUCCESS;
}
