#CC=gcc
CC=arm-linux-gnueabihf-gcc
CFLAGS=-g -std=c99 -pedantic -Wall
LDFLAGS=-lrt -lpthread

%.o:%.c
	$(CC) $(CFLAGS) -c $<

all: mzapo_binrep

mzapo_binrep: mzapo_binrep.o
	$(CC) $(CFLAGS) $(LDFLAGS) -L. $^ -o $@


.PHONY : depend dep all

dep:
	$(CC) $(CFLAGS) $(CPPFLAGS) -w -E -M *.c \
	  > depend

depend:
	@touch depend

clean:
	rm -f *.o *.a mzapo_binrep depend

-include depend
