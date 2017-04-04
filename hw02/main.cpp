#include <iostream>
#include <string.h>

void print(char *text){
    std::cout << text << std::endl;
}

int main(int argc, char* argv[]) {
    char *fname;

    if(argc > 1){
        fname  = argv[1];
    } else {
        return EXIT_FAILURE;
    }

    FILE *f = fopen(fname, "w");
    if(!f) return EXIT_FAILURE;


    return 0;
}