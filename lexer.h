#ifndef _LEXER_H_
#define _LEXER_H_

#include <string>
#include <vector>
#include <regex>
#include <utility>


#define token_regex(s) regex(s)

enum TokenType {
  BLANK = 0,
  INT_TYPE = 1,
  FOR = 2,
  IF = 3,
  DO = 4,
  WHILE = 5,
  BREAK = 6,
  PRINTF = 7,
  IDENTIFIER = 8,
  NUMBER = 9,
  RP = 10,
  LP = 11,
  LB = 12,
  RB = 13,
  ADD = 14,
  SUB = 15,
  MUL = 16,
  DIV = 17,
  DEC = 18,
  INC = 19,
  ASSIGNMENT = 20,
  NE = 21,
  GE = 22,
  LE = 23,
  GT = 24,
  LT = 25,
  EQ = 26,
  COLON = 27,
  COMMA = 28,
  SEMICOLON = 29,
  COMMENT = 30,
  CRLF = 31,
  STRING_LITERAL = 32,
};

class Token {
public:
 Token(enum TokenType type, int linum, std::string code, std::pair<int, int> span):
  type(type), linum(linum), code(code), begin(span.first), end(span.second) {};
  void print();
private:
  enum TokenType type;
  int linum;
  size_t begin;
  size_t end;
  std::string code;
};

class Lexer {
public:
  Lexer();
  int scan(std::string s, std::vector<Token>& result);
private:
  std::vector< std::pair<TokenType, std::regex> > tokenTpls;

  int count_crlf(std::string s);
};

#endif
