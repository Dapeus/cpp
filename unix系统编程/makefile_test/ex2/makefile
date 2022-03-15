CC = gcc
CFLAGS = -lm -Wall -g

main: main.c foo.o bar.o
	$(CC) $(CFLAGS) main.c foo.o bar.o -o main

foo.o: foo.c
	$(CC) $(CFLAGS) -c foo.c

bar.o: bar.c
	$(CC) $(CFLAGS) -c bar.c

clean:
	rm *.o main