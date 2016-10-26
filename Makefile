all: SeedCup

clean:
	rm *.o SeedCup

SeedCup : main.cpp parser.o lexer.o
	g++ parser.o lexer.o main.cpp -o SeedCup

parser.o :  parser.cpp parser.h
	g++ -c parser.cpp -o parser.o

lexer.o : lexer.cpp lexer.h
	g++ -c lexer.cpp -o lexer.o

