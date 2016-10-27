#ifndef _LEXER_H_
#define _LEXER_H_

#include <string>
#include <vector>
#include <regex>
#include <utility>


#define token_regex(s) regex(s)


class Token {
public:

  typedef enum {
    BLANK = 0,
  
    INT_TYPE = 10,
    FOR = 12,
    IF = 13,
    ELSE = 14,
    WHILE = 15,
    BREAK = 16,
    PRINTF = 17,
    DO = 18,

    IDENTIFIER = 30,
    NUMBER = 31,
    RP = 32,
    LP = 33,
    LB = 34,
    RB = 35,

    MUL = 41,
    DIV = 42,
    DEC = 43,
    INC = 44,
    ADD = 45,
    SUB = 46,
    ASSIGNMENT = 47,
  
    LE = 61,
    GT = 62,
    LT = 63,
    EQ = 64,
    NE = 65,
    GE = 66,

    COMMA = 81,
    SEMICOLON = 82,
    COLON = 83,

    COMMENT = 91,
    CRLF = 92,
    STRING_LITERAL = 93,
  } TokenType;
  
 Token(TokenType type, int linum, std::string code, std::pair<int, int> span):
  type(type), linum(linum), code(code), begin(span.first), end(span.second) {};
  void print();

  TokenType type;
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
  std::vector< std::pair<Token::TokenType, std::regex> > tokenTpls;
  

  int count_crlf(std::string s);
};

#endif
