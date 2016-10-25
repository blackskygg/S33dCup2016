#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <vector>
#include <regex>

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
};

#endif
