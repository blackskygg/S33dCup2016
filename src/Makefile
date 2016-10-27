CC=cc
CFLAGS=-Wall

all: main token syntax
	$(CC) $(CFLAGS) -o SeedCup2016.exe main.o token.o syntax.o

main: main.c
	$(CC) $(CFLAGS) -c main.c

token: token.c
	$(CC) $(CFLAGS) -c token.c

syntax: syntax.c
	$(CC) $(CFLAGS) -c syntax.c

clean:
	rm SeedCup2016.exe *.o
