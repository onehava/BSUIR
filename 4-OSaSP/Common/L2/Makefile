CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
LDFLAGS=

.PHONY: build parent son clean

build: parent son

# parent target rules
parent: parent.o
	$(CC) $(LDFLAGS) parent.o -o parent

parent.o:
	$(CC) $(CFLAGS) -c parent.c -o parent.o

# child target rules
son: son.o
	$(CC) $(LDFLAGS) son.o -o son

son.o:
	$(CC) $(CFLAGS) -c son.c -o son.o

# clean rule
clean:
	rm -r parent son parent.o son.o