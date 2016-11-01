#include <iostream>
#include <sstream>
#include "lexer.h"
#include "parser.h"

using namespace std;

/* read the raw content from file */
string read_file(const char *fn)
{
  ifstream ifs(fn, ios::binary);
  ostringstream ostrm;

  ostrm << ifs.rdbuf();
  return ostrm.str();
}



int main(int argc, char *argv[])
{
  string buf = read_file("input.txt");

  /* stage one: tokenize */
  Lexer lexer;
  vector<Token> tokens;
  lexer.scan(buf, tokens);

  /* stage two: parsing
   * this stage includes generating the result
   */
  Parser parser(tokens);
  Result result;
  parser.parse(result);

  /* stage tree: output */
  ofstream ofs("output.txt");
  result.print(ofs);

  return 0;
}
