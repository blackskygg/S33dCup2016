CC=cc
CFLAGS=-Wall

all: main token encode cp_test
	$(CC) $(CFLAGS) -o SeedCup2016.exe main.o token.o encode.o

main: main.c
	$(CC) $(CFLAGS) -c main.c

token: token.c
	$(CC) $(CFLAGS) -c token.c

encode: encode.c
	$(CC) $(CFLAGS) -c encode.c

cp_test:
	cp ../tests/input.txt .

clean:
	rm SeedCup2016.exe *.o input.txt
