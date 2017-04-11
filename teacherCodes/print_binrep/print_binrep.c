/* Simple program to examine how are different data types encoded in memory */

#include <stdio.h>

/*
 * The macro determines size of given variable and then
 * prints individual bytes of the value representation
 */
#define PRINT_MEM(a) print_mem((unsigned char*)&(a), sizeof(a))

void print_mem(unsigned char *ptr, int size) {
  int i;
  printf("address = 0x%08lx\n", (long unsigned int)ptr);

  for (i = 0; i < size; i++) {
    printf("0x%02x ", *(ptr+i));
  }

  printf("\n");
}

int main() {
  /* try for more types: long, float, double, pointer */
  unsigned int unsig = 5;
  int sig = -5;

  /* Read GNU C Library manual for conversion syntax for other types */
  /* https://www.gnu.org/software/libc/manual/html_node/Formatted-Output.html */
  printf("value = %d\n", unsig);
  PRINT_MEM(unsig);

  printf("\nvalue = %d\n", sig);
  PRINT_MEM(sig);

  return 0;
}
