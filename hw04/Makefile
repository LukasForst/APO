CC=gcc
#CC=arm-linux-gnueabihf-gcc
#CC=mips-elf-gcc
CFLAGS=-g -std=c99 -pedantic -Wall

%.o:%.c
	$(CC) $(CFLAGS) -c $<

all: program

program: main.o  subroutine.o  toplevel.o
	$(CC) $(CFLAGS) $(LDFLAGS) -L. $^ -o $@


.PHONY : depend dep all

dep:
	$(CC) $(CFLAGS) $(CPPFLAGS) -w -E -M *.c \
	  > depend

depend:
	@touch depend

clean:
	rm -f *.o *.a *.zip program depend
zip:
	zip hw04.zip program-mips.list subroutine.c toplevel.c main.c program-x86.list subroutine.h Makefile README.txt syscalls.lst

-include depend
