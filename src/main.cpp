#include "lexer.h"
#include "parser.h"

using namespace std;

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
  
  Lexer lexer;
  vector<Token> tokens;
  lexer.scan(buf, tokens);

  Parser parser(tokens);
  Result result;
  parser.parse(result);

  ofstream ofs("output.txt");
  result.print(ofs);
  
  return 0;
}
