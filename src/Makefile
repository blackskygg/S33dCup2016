CXX=g++
CFLAGS= -std=c++14 -g
OBJS = eval_expr.o execute_stat.o parse_stat.o 	\
	parse_expr.o parser.o lexer.o
all: 
	make -j `getconf _NPROCESSORS_ONLN` SeedCup

clean:
	rm -rf *.o SeedCup2016.exe *.dSYM

SeedCup : main.cpp $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) main.cpp -o SeedCup2016.exe

parser.o : parser.cpp parser.h 
	$(CXX) $(CFLAGS) -c parser.cpp -o parser.o

parse_stat.o : parse_stat.cpp parser.h
	$(CXX) $(CFLAGS) -c parse_stat.cpp -o parse_stat.o

parse_expr.o : parse_expr.cpp parser.h
	$(CXX) $(CFLAGS) -c parse_expr.cpp -o parse_expr.o

eval_expr.o : eval_expr.cpp parser.h
	$(CXX) $(CFLAGS) -c eval_expr.cpp -o eval_expr.o

execute_stat.o : execute_stat.cpp parser.h
	$(CXX) $(CFLAGS) -c execute_stat.cpp -o execute_stat.o

lexer.o : lexer.cpp lexer.h
	$(CXX) $(CFLAGS) -c lexer.cpp -o lexer.o
