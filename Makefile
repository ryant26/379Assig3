CC := gcc

CFLAGS := -Wall -g

proj3 : proj3.o
	$(CC) -o proj3 $<

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean :
	rm -f *.o proj3

proj3.o : proj3.c proj3.h