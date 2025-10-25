CC = gcc
CFLAGS = -Wall -g

all: library  

library: main.o library.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f library *.o
