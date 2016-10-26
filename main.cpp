#include <iostream>
#include <cstdio>
#include "lexer.h"
#include "parser.h"

using namespace std;

int main(int argc, char *argv[])
{
  Lexer lexer;
  Parser parser;
  char* buf = new char [4 * 1024 * 1024];
  size_t pos = 0, len;
  FILE *fp;


  if (argc < 2)
    return 0;

  if (! (fp = fopen(argv[1], "rb")) )
    return -1;
  
  while (!feof(fp)) {
    len = fread(buf + pos, 1, 512, fp);
    pos += len;
  }
  buf[pos] = '\0';

  vector<Token> tokens;
  lexer.scan(buf, tokens);
  for(auto token: tokens)
    token.print();

  vector<Statement> stats;
  parser.parse(tokens, stats);

  return 0;
}
