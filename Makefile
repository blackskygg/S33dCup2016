CXX=c++
CFLAGS= -std=c++14 -g
all: SeedCup

clean:
	rm *.o SeedCup2016.exe

SeedCup : main.cpp parser.o lexer.o
	$(CXX) $(CFLAGS) parser.o lexer.o main.cpp -o SeedCup2016.exe

parser.o :  parser.cpp parser.h
	$(CXX) $(CFLAGS) -c parser.cpp -o parser.o

lexer.o : lexer.cpp lexer.h
	$(CXX) $(CFLAGS) -c lexer.cpp -o lexer.o
