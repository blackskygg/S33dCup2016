#include <iostream>
#include "lexer.h"

using namespace std;

Lexer::Lexer()
{
  /* supported tokens and their patterns */
#define TOKEN_TPL(T, R) token_tpls.push_back(make_pair(Token::T, regex(R)))
  TOKEN_TPL(BLANK, "[ \t]+");
  TOKEN_TPL(INT_TYPE, "int");
  TOKEN_TPL(FOR, "for");
  TOKEN_TPL(IF, "if");
  TOKEN_TPL(ELSE, "else");
  TOKEN_TPL(DO, "do");
  TOKEN_TPL(WHILE, "while");
  TOKEN_TPL(BREAK, "break");
  TOKEN_TPL(PRINTF, "printf");
  TOKEN_TPL(IDENTIFIER, "[a-zA-Z_]\\w*");
  TOKEN_TPL(NUMBER, "\\d+");
  TOKEN_TPL(RP, "\\)");
  TOKEN_TPL(LP, "\\(");
  TOKEN_TPL(RB, "\\}");
  TOKEN_TPL(LB, "\\{");
  TOKEN_TPL(ADD, "\\+");
  TOKEN_TPL(SUB, "-");
  TOKEN_TPL(MUL, "\\*");
  TOKEN_TPL(DIV, "/");
  TOKEN_TPL(DEC, "--");
  TOKEN_TPL(INC, "\\+\\+");
  TOKEN_TPL(ASSIGNMENT, "=");
  TOKEN_TPL(EQ, "==");
  TOKEN_TPL(NE, "!=");
  TOKEN_TPL(GE, ">=");
  TOKEN_TPL(LE, "<=");
  TOKEN_TPL(GT, ">");
  TOKEN_TPL(LT, "<");
  TOKEN_TPL(SEMICOLON, ";");
  TOKEN_TPL(COLON, ":");
  TOKEN_TPL(COMMA, ",");
  TOKEN_TPL(COMMENT,"(/\\*[^]*?\\*/)|(//[^\r\n]*)");
  TOKEN_TPL(CRLF, "(?:\r\n)|(?:\n)");  //both CRLF and \n will work
  TOKEN_TPL(STRING_LITERAL, "\"(\\\\.|[^\"])*\"");
#undef TOKEN_TPL
}

/* count the number of newlines in s */
int Lexer::count_crlf(string s)
{
  size_t pos = 0;
  int result = 0;

  while (string::npos != (pos = s.find("\n", pos))) {
    result++;
    pos += 2;
  }

  return result;
}

/* main scanner
 * scan the raw input and return a list of Tokens
 */
int Lexer::scan(string s, vector<Token>& result)
{
  Token::TokenType type, last_type = Token::BLANK;
  smatch m;
  ssub_match best_match;
  int max_len = 0, linum = 1;
  auto str_begin = cbegin(s), str_end = cend(s);

  while (str_begin != str_end) {
    max_len = 0;
    //search for best match
    for (auto tpl: token_tpls) {
      if (regex_search(str_begin, str_end, m, tpl.second,
		       regex_constants::match_continuous)
	  && m[0].length() > max_len)
	{
	  best_match = m[0];
	  max_len = m[0].length();
	  type = tpl.first;
	}
    }

    if (!max_len) break;
    else str_begin += max_len;
    
    // increase the linum accordingly
    if (Token::CRLF == type) ++linum;
    else if (Token::COMMENT == type) linum += count_crlf(best_match.str());

    /* Ignore blank crlf and comments
     * And if encountered with consequtive string listerals, 
     * only push the first one to handle the "string" "string" concating syntax
     */
    if (type == Token::CRLF || type == Token::COMMENT || type == Token::BLANK
	|| (type == Token::STRING_LITERAL
	    && last_type == Token::STRING_LITERAL)) {
      continue;
    } else {
      last_type = type;
      result.push_back(Token(type, linum, best_match.str()));
    }
    
  }

  return 0;
}
