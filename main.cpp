#include <iostream>
#include <cstdio>
#include "lexer.h"
#include "parser.h"

using namespace std;

int read_file(char *fn, char *buf)
{
  size_t pos = 0, len;
  FILE *fp;
  if (! (fp = fopen(fn, "rb")) )
    return -1;
  
  while (!feof(fp)) {
    len = fread(buf + pos, 1, 512, fp);
    pos += len;
  }
  buf[pos] = '\0';

  fclose(fp);
  return 0;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
    return 0;
  
  char* buf = new char [4 * 1024 * 1024];

  if(0 != read_file(argv[1], buf))
    return -1;

  Lexer lexer;
  vector<Token> tokens;
  lexer.scan(buf, tokens);
  /*
  for(auto token: tokens)
    token.print();
  */

  Parser parser(tokens);
  Result result;
  parser.parse(result);
  result.print();

  delete[] buf;
  
  return 0;
}
