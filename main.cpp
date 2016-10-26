#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"
#include "parser.h"


int main(int argc, char *argv[])
{
  Lexer lexer;
  Parser parser;
  char *buf = (char *)malloc(4 * 1024 * 1024);
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
  
  lexer.scan(buf);
  
  return 0;
}
