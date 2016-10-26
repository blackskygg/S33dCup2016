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
  PRINTF = 6,
  IDENTIFIER = 7,
  NUMBER = 8,
  RP = 9,
  LP = 10,
  LB = 11,
  RB = 12,
  ADD = 13,
  SUB = 14,
  MUL = 15,
  DIV = 16,
  DEC = 17,
  INC = 18,
  ASSIGNMENT = 19,
  EQ = 20,
  NE = 21,
  GE = 22,
  LE = 23,
  GT = 24,
  LT = 25,
  SEMICOLON = 26,
  COLON = 27,
  COMMA = 28,
  COMMENT = 29,
  CRLF = 30,
  STRING_LITERAL = 31,
};

class Token {
public:
 Token(enum TokenType type, int linum, std::pair<int, int> span):
  type(type), linum(linum), begin(span.first), end(span.second) {};
private:
  enum TokenType type;
  int linum;
  int begin;
  int end;
  int code;
};

class Lexer {
public:
  Lexer();
  std::vector<Token> scan(std::string s);
private:
  std::vector< std::pair<TokenType, std::regex> > tokenTpls;

  int count_crlf(std::string s);
};

#endif
