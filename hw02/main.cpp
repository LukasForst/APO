#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#define OUTPUT "output.ppm"
#define HISTOGRAM "output.txt"

int main(int argc, char *argv[]) {
    FILE *old_file = fopen(argv[1], "rb");

    register int width, height, color_max, error;
    error = fscanf(old_file, "P6\n%d\n%d\n%d\n", &width, &height, &color_max);

    register int current_pixel,
            idx = 0,
            array_width = width * 3,
            size = array_width*height,
            active_column = 1;

    //allocate and read
    register unsigned char *old_ppm = (unsigned char *) malloc(sizeof(unsigned char) * size);
    error = fread(old_ppm, sizeof(unsigned char), (size_t) size, old_file);
    fclose(old_file);
    register unsigned char *solution = (unsigned char *) malloc(sizeof(unsigned char) * size);

    register int gray_scale[] = {0, 0, 0, 0, 0, 0};
    register int rounded;
    register double ratio[] = {0.2126, 0.7152, 0.0722};
    register double gray = 0;

    //first row
    for(;idx < array_width;){
        gray += old_ppm[idx] * ratio[0];
        solution[idx] = old_ppm[idx];
        idx++;

        gray += old_ppm[idx] * ratio[1];
        solution[idx] = old_ppm[idx];
        idx++;

        gray += old_ppm[idx] * ratio[2];
        solution[idx] = old_ppm[idx];
        idx++;

        rounded = round(gray);
        gray = 0;
        gray_scale[(int) rounded / 51]++;
    }

    register int fo_end = size - array_width;
    for (;idx < fo_end;) {
        if (active_column == width || active_column == 1) {
            gray += old_ppm[idx] * ratio[0];
            solution[idx] = old_ppm[idx];
            idx++;

            gray += old_ppm[idx] * ratio[1];
            solution[idx] = old_ppm[idx];
            idx++;

            gray += old_ppm[idx] * ratio[2];
            solution[idx] = old_ppm[idx];
            idx++;

            active_column = active_column == width ? 1 : active_column + 1;
        } else {
            for(int i = 0; i < 3; ++i, ++idx){
                current_pixel = -old_ppm[idx - array_width];
                current_pixel -= old_ppm[idx-3];
                current_pixel += 5 * old_ppm[idx];
                current_pixel -= old_ppm[idx+3];
                current_pixel -= old_ppm[idx+array_width];

                current_pixel = current_pixel > 255 ? 255 : current_pixel;
                current_pixel = current_pixel < 0 ? 0 : current_pixel;

                //gray scale computation
                gray += ratio[i]*current_pixel;
                solution[idx] = current_pixel;
            }
            ++active_column;
        }

        rounded = round(gray);
        gray = 0;
        gray_scale[(int) rounded / 51]++;
    }

    //last row
    for(;idx < size;){
        gray += old_ppm[idx] * ratio[0];
        solution[idx] = old_ppm[idx];
        idx++;

        gray += old_ppm[idx] * ratio[1];
        solution[idx] = old_ppm[idx];
        idx++;

        gray += old_ppm[idx] * ratio[2];
        solution[idx] = old_ppm[idx];
        idx++;

        ++active_column;

        rounded = round(gray);
        gray = 0;
        gray_scale[(int) rounded / 51]++;
    }

    free(old_ppm);
    FILE *new_ppm = fopen(OUTPUT, "wb");
    fprintf(new_ppm, "P6\n%d\n%d\n%d\n", width, height, color_max);
    fwrite(solution, size, 1, new_ppm);
    fclose(new_ppm);
    free(solution);

    FILE *histogram = fopen(HISTOGRAM, "w");
    fprintf(histogram, "%d %d %d %d %d", gray_scale[0], gray_scale[1], gray_scale[2], gray_scale[3],
            gray_scale[4] + gray_scale[5]);
    fclose(histogram);
    error = 0;
    return error;
}