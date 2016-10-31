#include <iostream>
#include <sstream>
#include "lexer.h"
#include "parser.h"

using namespace std;

/* read the raw content from file */
string read_file(char *fn)
{
  ifstream ifs(fn, ios::binary);
  ostringstream ostrm;

  ostrm << ifs.rdbuf();
  return ostrm.str();
}
    


int main(int argc, char *argv[])
{
  if (argc < 2)
    return 0;

  string buf = read_file(argv[1]);

  /* stage one: tokenize */
  Lexer lexer;
  vector<Token> tokens;
  lexer.scan(buf, tokens);
  for (auto tk : tokens)
    tk.print();

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
