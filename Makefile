CC = gcc
CFLAGS = -Wall -ansi  -g

all: library  

library: main.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f library *.o
