CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
LDFLAGS=

.PHONY: build clean

build: main.o
	$(CC) $(LDFLAGS) main.o -o dirwalk

main.o:
	$(CC) $(CFLAGS) -c main.c -o main.o

clean: 
	rm -r dirwalk main.o