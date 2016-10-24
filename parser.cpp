#include <regex>
#include <string>
#include <iostream>
#include <utility>
#include <cstdio>
#include "parser.h"

using namespace std;

class Token {
public:
  Token(enum TokenType type, int linum, pair<int, int> span):
    type(type), linum(linum), begin(begin), end(end) {};
private:
  enum TokenType type;
  int begin;
  int end;
  int linum;
  int code;
};

class Parser {
public:
  Parser();
  vector<Token> parse(string s);
private:
  vector< pair<TokenType, regex> > tokenTpls;
};


Parser::Parser()
{
  tokenTpls.push_back(make_pair(BLANK, token_regex("[ \t]+")));
  tokenTpls.push_back(make_pair(INT_TYPE, token_regex("int")));
  tokenTpls.push_back(make_pair(IDENTIFIER, token_regex("[a-zA-Z_]\\w*")));
  tokenTpls.push_back(make_pair(NUMBER, token_regex("\\d+")));
  tokenTpls.push_back(make_pair(RB, token_regex("\\)")));
  tokenTpls.push_back(make_pair(LB, token_regex("\\(")));
  tokenTpls.push_back(make_pair(ADD, token_regex("\\+")));
  tokenTpls.push_back(make_pair(SUB, token_regex("-")));
  tokenTpls.push_back(make_pair(STAR, token_regex("\\*")));
  tokenTpls.push_back(make_pair(DEC, token_regex("--")));
  tokenTpls.push_back(make_pair(INC, token_regex("\\+\\+")));
  tokenTpls.push_back(make_pair(ASSIGNMENT, token_regex("=")));
  tokenTpls.push_back(make_pair(EQUAL, token_regex("=")));
  tokenTpls.push_back(make_pair(GE, token_regex(">=")));
  tokenTpls.push_back(make_pair(LE, token_regex("<=")));
  tokenTpls.push_back(make_pair(GT, token_regex(">")));
  tokenTpls.push_back(make_pair(LT, token_regex("<")));
  tokenTpls.push_back(make_pair(PRINTF, token_regex("printf")));
  tokenTpls.push_back(make_pair(SEMICOLON, token_regex(";")));
  tokenTpls.push_back(make_pair(COLON, token_regex(":")));
  tokenTpls.push_back(make_pair(COMMENT, token_regex("(/\\*.*?\\*/)|(//[^\r\n]*)")));
  tokenTpls.push_back(make_pair(NEWLINE, token_regex("\r\n")));
  tokenTpls.push_back(make_pair(STRING_LITERAL, token_regex("\"(\\\\.|[^\"])*\"")));
}

vector<Token> Parser::parse(string s)
{
  enum TokenType type;
  smatch m;
  ssub_match best_match;
  int max_len = 0, linum = 0;
  vector<Token> result;
  auto str_begin = cbegin(s), str_end = cend(s);
  decltype(str_begin) temp_it;

  while (str_begin < str_end) {
    
    temp_it = str_begin;
    max_len = 0;
    for (auto tpl: tokenTpls) {
      if (regex_search(str_begin, str_end, m, tpl.second, regex_constants::match_continuous)
	  && m[0].length() >= max_len) {
	best_match = m[0];
	max_len = m[0].length();
	type = tpl.first;
      }
      str_begin = temp_it;
    }

    if (max_len) {
      str_begin = temp_it + max_len;
      cout<<"{type: "<< type <<", length: "<< max_len<<"}"<<endl;
    } else {
      break;
    }

  }
  
  return result;
}

int main(int argc, char *argv[])
{
  Parser parser;
  string s;
  char buf[256];

  for(;;) {
    fgets(buf, 256, stdin);
    parser.parse(buf);
  }
  
  return 0;
}
