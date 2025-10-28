CC = gcc
CFLAGS = -Wall -g -pedantic

all: library  

library: main.o library.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f library *.o
