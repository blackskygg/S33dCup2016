#include <regex>
#include <string>
#include <iostream>
#include <utility>
#include <cstdio>
#include <cstdlib>
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

  int count_crlf(string s);
};


Parser::Parser()
{
  tokenTpls.push_back(make_pair(BLANK, token_regex("[ \t]+")));
  tokenTpls.push_back(make_pair(INT_TYPE, token_regex("int")));
  tokenTpls.push_back(make_pair(FOR, token_regex("for")));
  tokenTpls.push_back(make_pair(IF, token_regex("if")));
  tokenTpls.push_back(make_pair(DO, token_regex("do")));
  tokenTpls.push_back(make_pair(WHILE, token_regex("while")));
  tokenTpls.push_back(make_pair(PRINTF, token_regex("printf")));
  tokenTpls.push_back(make_pair(IDENTIFIER, token_regex("[a-zA-Z_]\\w*")));
  tokenTpls.push_back(make_pair(NUMBER, token_regex("\\d+")));
  tokenTpls.push_back(make_pair(RP, token_regex("\\)")));
  tokenTpls.push_back(make_pair(LP, token_regex("\\(")));
  tokenTpls.push_back(make_pair(RB, token_regex("\\}")));
  tokenTpls.push_back(make_pair(LB, token_regex("\\{")));
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
  tokenTpls.push_back(make_pair(SEMICOLON, token_regex(";")));
  tokenTpls.push_back(make_pair(COLON, token_regex(":")));
  tokenTpls.push_back(make_pair(COMMA, token_regex(",")));
  tokenTpls.push_back(make_pair(COMMENT, token_regex("(/\\*.*?\\*/)|(//[^\r\n]*)")));
  tokenTpls.push_back(make_pair(CRLF, token_regex("\r\n")));
  tokenTpls.push_back(make_pair(STRING_LITERAL, token_regex("\"(\\\\.|[^\"])*\"")));
  
}

int Parser::count_crlf(string s)
{
  size_t pos = 0, len;
  int result = 0;
  while (string::npos != (len = s.find("\r\n", pos))) {
    result++;
    pos+=len;
  }

  return result;
}

vector<Token> Parser::parse(string s)
{
  enum TokenType type;
  smatch m;
  ssub_match best_match;
  int max_len = 0, linum = 1;
  vector<Token> result;
  auto str_begin = cbegin(s), str_end = cend(s);

  while (str_begin != str_end) {
    max_len = 0;

    //search for best match
    for (auto tpl: tokenTpls) {
      if (regex_search(str_begin, str_end, m, tpl.second, regex_constants::match_continuous)
	  && m[0].length() > max_len) {
	best_match = m[0];
	max_len = m[0].length();
	type = tpl.first;
      }
    }

    //output the match result
    if (max_len) {
      str_begin += max_len;
      cout<<"{type: "<< type <<", length: "<< max_len
	  <<", code:  \""<<best_match.str()
	  <<"\", linum: "<< linum<<"}"<<endl;
    } else {
      break;
    }

    //add the linum accordingly
    if (CRLF == type) {
      ++linum;
    } else if (COMMENT == type) {
      linum += count_crlf(best_match.str());
    }

  }
  
  return result;
}


int main(int argc, char *argv[])
{
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
  
  parser.parse(buf);
  
  return 0;
}
