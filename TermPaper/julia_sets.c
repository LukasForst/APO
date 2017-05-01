#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define DEFAULT_ITERATIONS 100

typedef struct {
    char red;
    char green;
    char blue;
}color;

color ** generate_julia(int width, int height, double move_x, double move_y, double c_real, double c_imag, int max_iterations){
    max_iterations = max_iterations == 0 ? DEFAULT_ITERATIONS : max_iterations;

    double new_real, new_imag, old_real, old_imag;   //real and imaginary parts of new and old z

    color ** result = (color **)calloc(width*height ,sizeof(color**));
    int result_idx = 0;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++)
        {
            new_real = 1.5 * (x - width / 2) / (0.5 * width) + move_x;
            new_imag = (y - height / 2) / (0.5 * height) + move_y;

            int i; // number of iterations
            for(i = 0; i < max_iterations; i++)
            {
                old_real = new_real;
                old_imag = new_imag;

                new_real = old_real * old_real - old_imag * old_imag + c_real;
                new_imag = 2 * old_real * old_imag + c_imag;

                if(fabs(new_real * new_real + new_imag * new_imag) > 2) break;

            }
            color *c = (color*)calloc(1, sizeof(color));

            double t = (double)i/(double)max_iterations;

            c->red = (char)(9*(1-t)*t*t*t*255);;
            c->green = (char)(15*(1-t)*(1-t)*t*t*255);
            c ->blue = (char)(8.5*(1-t)*(1-t)*(1-t)*t*255);

            *(result + result_idx++) = c;
        }
    }
    return result;
}

color ** generate_mandelbrot(int width, int height, double move_x, double move_y, int max_iterations){
    max_iterations = max_iterations == 0 ? DEFAULT_ITERATIONS : max_iterations;

    double pixel_imag, pixel_real;
    double new_real, new_imag, old_real, old_imag;   //real and imaginary parts of new and old z

    color ** result = (color **)calloc(width*height ,sizeof(color**));
    int result_idx = 0;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++)
        {
            pixel_real = 1.5 * (x - width / 2) / (0.5 * width) + move_x;
            pixel_imag = (y - height / 2) / (0.5 * height) + move_y;
            new_real = new_imag = old_imag = old_real = 0;

            int i; // number of iterations
            for(i = 0; i < max_iterations; i++)
            {
                old_real = new_real;
                old_imag = new_imag;

                new_real = old_real * old_real - old_imag * old_imag + pixel_real;
                new_imag = 2 * old_real * old_imag + pixel_imag;

                if(fabs(new_real * new_real + new_imag * new_imag) > 2) break;

            }
            color *c = (color*)calloc(1, sizeof(color));

            double t = (double)i/(double)max_iterations;

            c->red = (char)(9*(1-t)*t*t*t*255);;
            c->green = (char)(15*(1-t)*(1-t)*t*t*255);
            c ->blue = (char)(8.5*(1-t)*(1-t)*(1-t)*t*255);

            *(result + result_idx++) = c;
        }
    }
    return result;
}


int main(){
    int width = 800, height = 600;
    double c_real = -0.7, c_imag = 0.27015,  move_x = -0.5, move_y = 0;
    //double c_real = -0.4, c_imag = 0;

    FILE *fp = fopen("pic.ppm", "wb");
    fprintf(fp, "P6\n%d\n%d\n255\n",width,height);

    //color ** j = generate_julia(width, height, move_x, move_y, c_real, c_imag, 100);
    color ** j = generate_mandelbrot(width, height, move_x, move_y, 400);

    for(int i =0; i < width*height; i++){
        fwrite(*(j+i), sizeof(color), 1, fp);
        free(*(j+i));
    }
    free(j);

    fclose(fp);
}