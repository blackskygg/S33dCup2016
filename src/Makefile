CC=clang
CFLAGS=-Wall -g

all:
	make -j `getconf _NPROCESSORS_ONLN` compile_all

compile_all: main.o token.o syntax.o evaluate.o
	$(CC) $(CFLAGS) -o SeedCup2016.exe main.o token.o syntax.o evaluate.o

main: main.c
	$(CC) $(CFLAGS) -c main.c

token: token.c
	$(CC) $(CFLAGS) -c token.c

syntax: syntax.c
	$(CC) $(CFLAGS) -c syntax.c

evaluate: evaluate.c
	$(CC) $(CFLAGS) -c evaluate.c

clean:
	rm SeedCup2016.exe *.o
