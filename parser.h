#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <vector>
#include <regex>

#define token_regex(s) regex(s)

enum TokenType {
  BLANK = 0,
  INT_TYPE = 1,
  IDENTIFIER = 2,
  NUMBER = 3,
  RB = 4,
  LB = 5,
  ADD = 6,
  SUB = 7,
  STAR = 8,
  DEC = 9,
  INC = 10,
  ASSIGNMENT = 11,
  EQUAL = 12,
  GE = 13,
  LE = 14,
  GT = 15,
  LT = 16,
  PRINTF = 17,
  SEMICOLON = 18,
  COLON = 19,
  COMMA = 20,
  COMMENT = 21,
  NEWLINE = 22,
  STRING_LITERAL = 23,
};

#endif
