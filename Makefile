CC=gcc
CFLAGS = -std=c99 -Wall -g 

all: shttp

shttp: main.o http.o modules/auth.o modules/firewall.o modules/logger.o mime.o
	$(CC) $(CFLAGS) -o main main.o http.o modules/auth.o modules/firewall.o modules/logger.o mime.o

main.o: main.c http.o
	$(CC) $(CFLAGS) -c main.c

http.o: http.c mime.o
	$(CC) $(CFLAGS) -c http.c

mime.o: mime.c
	$(CC) $(CFLAGS) -c mime.c