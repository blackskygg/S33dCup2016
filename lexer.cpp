#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"

using namespace std;

void Token::print()
{

  cout<<"{type: "<< type
      <<", pos: "<<begin<<", "<< end
      <<", code:  \""<<code
      <<"\", linum: "<< linum
      <<"}"<<endl;
}

Lexer::Lexer()
{
  tokenTpls.push_back(make_pair(Token::BLANK, token_regex("[ \t]+")));
  tokenTpls.push_back(make_pair(Token::INT_TYPE, token_regex("int")));
  tokenTpls.push_back(make_pair(Token::FOR, token_regex("for")));
  tokenTpls.push_back(make_pair(Token::IF, token_regex("if")));
  tokenTpls.push_back(make_pair(Token::ELSE, token_regex("else")));
  tokenTpls.push_back(make_pair(Token::DO, token_regex("do")));
  tokenTpls.push_back(make_pair(Token::WHILE, token_regex("while")));
  tokenTpls.push_back(make_pair(Token::BREAK, token_regex("break")));
  tokenTpls.push_back(make_pair(Token::PRINTF, token_regex("printf")));
  tokenTpls.push_back(make_pair(Token::IDENTIFIER, token_regex("[a-zA-Token::Z_]\\w*")));
  tokenTpls.push_back(make_pair(Token::NUMBER, token_regex("\\d+")));
  tokenTpls.push_back(make_pair(Token::RP, token_regex("\\)")));
  tokenTpls.push_back(make_pair(Token::LP, token_regex("\\(")));
  tokenTpls.push_back(make_pair(Token::RB, token_regex("\\}")));
  tokenTpls.push_back(make_pair(Token::LB, token_regex("\\{")));
  tokenTpls.push_back(make_pair(Token::ADD, token_regex("\\+")));
  tokenTpls.push_back(make_pair(Token::SUB, token_regex("-")));
  tokenTpls.push_back(make_pair(Token::MUL, token_regex("\\*")));
  tokenTpls.push_back(make_pair(Token::DIV, token_regex("/")));
  tokenTpls.push_back(make_pair(Token::DEC, token_regex("--")));
  tokenTpls.push_back(make_pair(Token::INC, token_regex("\\+\\+")));
  tokenTpls.push_back(make_pair(Token::ASSIGNMENT, token_regex("=")));
  tokenTpls.push_back(make_pair(Token::EQ, token_regex("==")));
  tokenTpls.push_back(make_pair(Token::NE, token_regex("!=")));
  tokenTpls.push_back(make_pair(Token::GE, token_regex(">=")));
  tokenTpls.push_back(make_pair(Token::LE, token_regex("<=")));
  tokenTpls.push_back(make_pair(Token::GT, token_regex(">")));
  tokenTpls.push_back(make_pair(Token::LT, token_regex("<")));
  tokenTpls.push_back(make_pair(Token::SEMICOLON, token_regex(";")));
  tokenTpls.push_back(make_pair(Token::COLON, token_regex(":")));
  tokenTpls.push_back(make_pair(Token::COMMA, token_regex(",")));
  tokenTpls.push_back(make_pair(Token::COMMENT,				\
				token_regex("(/\\*(.|\n)*?\\*/)|(//[^\n]*)")));
  tokenTpls.push_back(make_pair(Token::CRLF, token_regex("\n")));
  tokenTpls.push_back(make_pair(Token::STRING_LITERAL,			\
				token_regex("\"(\\\\.|[^\"])*\"")));
}

int Lexer::count_crlf(string s)
{
  size_t pos = 0;
  int result = 0;
  while (string::npos != (pos = s.find("\n", pos))) {
    result++;
    pos+=2;
  }

  return result;
}

int Lexer::scan(string s, vector<Token>& result)
{
  Token::TokenType type;
  smatch m;
  ssub_match best_match;
  int max_len = 0, linum = 1;
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

    if (!max_len)
      break;
    else
      str_begin += max_len;
    
    //add the linum accordingly
    if (Token::CRLF == type)
      ++linum;
    else if (Token::COMMENT == type)
      linum += count_crlf(best_match.str());


    if(type != Token::CRLF && type != Token::COMMENT && type != Token::BLANK) {
      size_t begin_pos = str_begin - cbegin(s);
      result.push_back(Token(type, linum, best_match.str(),
			     make_pair(begin_pos, begin_pos + best_match.str().length())));

    }
  }

  return 0;
}
