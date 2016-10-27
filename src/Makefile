CC=cc
CFLAGS=-Wall

all: main token
	$(CC) $(CFLAGS) -o SeedCup2016.exe main.o token.o

main: main.c
	$(CC) $(CFLAGS) -c main.c

token: token.c
	$(CC) $(CFLAGS) -c token.c

clean:
	rm SeedCup2016.exe *.o
