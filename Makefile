CC=gcc
CFLAGS = -std=c99 -Wall -Werror -g 

all: shttp

shttp: main.o http.o socketio.o modules/auth.o modules/firewall.o modules/logger.o
	$(CC) $(CFLAGS) -o main main.o http.o socketio.o modules/auth.o modules/firewall.o modules/logger.o

main.o: main.c http.o socketio.o:
	$(CC) $(CFLAGS) -c main.c

http.o: http.c
	$(CC) $(CFLAGS) -c http.c