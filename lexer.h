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
  STAR = 15,
  DEC = 16,
  INC = 17,
  ASSIGNMENT = 18,
  EQUAL = 19,
  GE = 20,
  LE = 21,
  GT = 22,
  LT = 23,
  SEMICOLON = 24,
  COLON = 25,
  COMMA = 26,
  COMMENT = 27,
  CRLF = 28,
  STRING_LITERAL = 29,
  NOT_EQU = 30,
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
