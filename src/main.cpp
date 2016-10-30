#include "lexer.h"
#include "parser.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc < 2)
    return 0;
  
  ifstream ifs(argv[1], ios::binary);
  string buf((istreambuf_iterator<char>(ifs)),
	     istreambuf_iterator<char>());
  
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
