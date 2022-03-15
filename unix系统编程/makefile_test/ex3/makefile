CC = gcc
CFLAGS = -lm -Wall -g

all: main_max main_min

main_max: main_max.c foo.o bar.o
	$(CC) $(CFLAGS) main_max.c foo.o bar.o -o main_max

main_min: main_min.c foo.o bar.o
	$(CC) $(CFLAGS) main_min.c foo.o bar.o -o main_min
foo.o: foo.c
	$(CC) $(CFLAGS) -c foo.c

bar.o: bar.c
	$(CC) $(CFLAGS) -c bar.c

clean:
	rm *.o main_max main_min