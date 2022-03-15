CC = gcc
CFLAGS = -Wall -g
THREAD = -lpthread

all:client server

server:server.c
	$(CC) $(CFLAGS) server.c -o server $(THREAD)

client:client.c
	$(CC) $(CFLAGS) client.c -o client $(THREAD)

clean:
	rm server client
