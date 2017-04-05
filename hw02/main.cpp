#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#define OUTPUT "output.ppm"
#define HISTOGRAM "output.txt"

int main(int argc, char *argv[]) {
    FILE *old_file = argc > 1 ? fopen(argv[1], "rb") : NULL;
    if(!old_file)
        return EXIT_FAILURE;

    int width, height, color_max;
    if (fscanf(old_file, "P6%d %d %d", &width, &height, &color_max) == 0)
        return EXIT_FAILURE;

    register int current_pixel,
            idx = 0,
            array_width = width * 3,
            size = array_width*height,
            active_row = 1;

    //allocate and read
    unsigned char *old_ppm = (unsigned char*)malloc(sizeof(unsigned char) * size);
    if (fread(old_ppm, sizeof(unsigned char), size, old_file) == 0)
        return EXIT_FAILURE;
    fclose(old_file);

    int gray_scale[] = {0,0,0,0,0};
    double ratio[] = {0.2126, 0.7152, 0.0722};

    register double gray = 0;
    //write image
    FILE *new_ppm = fopen(OUTPUT, "wb");
    fprintf(new_ppm, "P6\n%d\n%d\n%d", width, height, color_max);



    for (;idx != size; gray = 0) {
        if (active_row == width || active_row == 1 || idx < array_width || idx >= (array_width*height-array_width)) {
            gray += old_ppm[idx] * ratio[0];
            fputc(old_ppm[idx++], new_ppm);

            gray += old_ppm[idx] * ratio[1];
            fputc(old_ppm[idx++], new_ppm);

            gray += old_ppm[idx] * ratio[2];
            fputc(old_ppm[idx++], new_ppm);

            active_row = active_row == width ? 1 : active_row + 1;

        } else {
            for(int i = 0; i < 3; ++i, ++idx){
                current_pixel = 5*old_ppm[idx];
                current_pixel -= old_ppm[idx-3];
                current_pixel -= old_ppm[idx-array_width];
                current_pixel -= old_ppm[idx+3];
                current_pixel -= old_ppm[idx+array_width];

                current_pixel = current_pixel > 255 ? 255 : current_pixel;
                current_pixel = current_pixel < 0 ? 0 : current_pixel;

                //gray scale computation
                gray += ratio[i]*current_pixel;

                fputc((char)current_pixel, new_ppm);
            }
            ++active_row;
        }
        double rounded = round(gray);
        if(rounded < 51){
            gray_scale[0]++;
        } else if(rounded < 102){
            gray_scale[1]++;
        } else if(rounded < 153){
            gray_scale[2]++;
        } else if(rounded < 204){
            gray_scale[3]++;
        } else {
            gray_scale[4]++;
        }

    }
    free(old_ppm);
    FILE *histogram = fopen(HISTOGRAM, "w");
    fprintf(histogram, "%d %d %d %d %d", gray_scale[0], gray_scale[1], gray_scale[2], gray_scale[3], gray_scale[4]);

    fclose(new_ppm);
    fclose(histogram);

    return EXIT_SUCCESS;
}
