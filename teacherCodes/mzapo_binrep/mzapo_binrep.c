/*******************************************************************
  Simple program to demostrate binary reprezentation on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  mzapo_binrep.c       - main and only file

  (C) Copyright 2004 - 2017 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <byteswap.h>
#include <getopt.h>
#include <inttypes.h>
#include <time.h>

char *memdev="/dev/mem";

#define SPILED_REG_BASE_PHYS 0x43c40000
#define SPILED_REG_SIZE      0x00004000

#define SPILED_REG_LED_LINE_o           0x004
#define SPILED_REG_LED_RGB1_o           0x010
#define SPILED_REG_LED_RGB2_o           0x014
#define SPILED_REG_LED_KBDWR_DIRECT_o   0x018

#define SPILED_REG_KBDRD_KNOBS_DIRECT_o 0x020
#define SPILED_REG_KNOBS_8BIT_o         0x024


void *map_phys_address(off_t region_base, size_t region_size, int opt_cached)
{
  unsigned long mem_window_size;
  unsigned long pagesize;
  unsigned char *mm;
  unsigned char *mem;
  int fd;

  fd = open(memdev, O_RDWR | (!opt_cached? O_SYNC: 0));
  if (fd < 0) {
    fprintf(stderr, "cannot open %s\n", memdev);
    return NULL;
  }

  pagesize=sysconf(_SC_PAGESIZE);

  mem_window_size = ((region_base & (pagesize-1)) + region_size + pagesize-1) & ~(pagesize-1);

  mm = mmap(NULL, mem_window_size, PROT_WRITE|PROT_READ,
              MAP_SHARED, fd, region_base & ~(pagesize-1));
  mem = mm + (region_base & (pagesize-1));

  if (mm == MAP_FAILED) {
    fprintf(stderr,"mmap error\n");
    return NULL;
  }

  return mem;
}

int main(int argc, char *argv[])
{

  unsigned char *mem_base;

  mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

  if (mem_base == NULL)
    exit(1);

  while (1) {
     uint32_t rgb_knobs_value;
     int int_val;
     unsigned int uint_val;

     struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 200 * 1000 * 1000};

     rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);

     *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = rgb_knobs_value;

     *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = rgb_knobs_value;

     *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = rgb_knobs_value;

     int_val = rgb_knobs_value;
     uint_val = rgb_knobs_value;

     printf("int %10d uint 0x%08x\n", int_val, uint_val);

     clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }

  return 0;
}
