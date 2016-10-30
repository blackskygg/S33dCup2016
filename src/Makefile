CC=clang
CFLAGS=-Wall -g

all:
	make -j `getconf _NPROCESSORS_ONLN` compile_all

compile_all: main.o lexer.o parser.o evaluate.o
	$(CC) $(CFLAGS) -o SeedCup2016.exe main.o lexer.o parser.o evaluate.o

main: main.c
	$(CC) $(CFLAGS) -c main.c

lexer: lexer.c
	$(CC) $(CFLAGS) -c lexer.c

parser: parser.c
	$(CC) $(CFLAGS) -c parser.c

evaluate: evaluate.c
	$(CC) $(CFLAGS) -c evaluate.c

clean:
	rm SeedCup2016.exe *.o
