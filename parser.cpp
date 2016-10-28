#include "parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Identifier& Scope::get_identifier(string name)
{
}

int PrimaryExprConst:: eval(Scope& scope)
{
  return val;
}

int PrimaryExprId:: eval(Scope& scope)
{
  return scope.get_identifier(id).val;
}

int AssignmentExpr::eval(Scope& scope)
{
  return scope.get_identifier(id).val = expr->eval(scope);
}

int DeclStat::execute()
{
  return 0;
}

int CompoundStat::execute()
{
  for (auto stat: stat_list)
    stat->execute();
   
  return 0;
}

Parser::Parser(vector <Token> &tokens): tokens(tokens)
{
  code_map[Token::INT_TYPE] = 'i';
  code_map[Token::FOR] = 'f';
  code_map[Token::IF] = '?';
  code_map[Token::ELSE] = '@';
  code_map[Token::DO] = 'd';
  code_map[Token::WHILE] = 'w';
  code_map[Token::BREAK] = 'b';
  code_map[Token::PRINTF] = 'p';
  code_map[Token::IDENTIFIER] = 'a';
  code_map[Token::NUMBER] = 'n';
  code_map[Token::RP] = ')';
  code_map[Token::LP] = '(';
  code_map[Token::LB] = '{';
  code_map[Token::RB] = '}';
  code_map[Token::ADD] = '+';
  code_map[Token::SUB] = '-';
  code_map[Token::MUL] = '*';
  code_map[Token::DIV] = '/';
  code_map[Token::DEC] = 'v';
  code_map[Token::INC] = '^';
  code_map[Token::ASSIGNMENT] = '=';
  code_map[Token::NE] = '!';
  code_map[Token::GE] = 'g';
  code_map[Token::LE] = 'l';
  code_map[Token::GT] = '>';
  code_map[Token::LT] = '<';
  code_map[Token::EQ] = 'e';
  code_map[Token::COLON] = ':';
  code_map[Token::COMMA] = ',';
  code_map[Token::SEMICOLON] = ';';
  code_map[Token::STRING_LITERAL] = 's';

  expr_tpls.push_back(make_pair(Expression::COMMA, expr_regex("(.+),(.+)")));
  expr_tpls.push_back(make_pair(Expression::ASSIGNMENT, expr_regex("(.+?)=(.+)")));
  expr_tpls.push_back(make_pair(Expression::EQUALITY, expr_regex("(.+)([!e])(.+)")));
  expr_tpls.push_back(make_pair(Expression::RELATIONAL, expr_regex("(.+)([gl><])(.+)")));
  expr_tpls.push_back(make_pair(Expression::ADDITIVE, expr_regex("(.+)([\\+-])(.+)")));
  expr_tpls.push_back(make_pair(Expression::MULT, expr_regex("(.+)([\\*/])(.+)")));
  expr_tpls.push_back(make_pair(Expression::UNARY, expr_regex("[\\+-](.+)")));
  expr_tpls.push_back(make_pair(Expression::POSTFIX, expr_regex("(.+)[\\^v]")));
  expr_tpls.push_back(make_pair(Expression::PRIMARY, expr_regex("([nas])")));
}

void Parser::encode_tokens(std::vector <Token>& tokens, string &s)
{
  for (auto token: tokens) 
    s.push_back(code_map[token.type]);
}


int Parser::parse(vector< shared_ptr<Statement> >& stats)
{
  string s;
  shared_ptr<Scope> global_scp;

  encode_tokens(tokens, s);
  parse_stat_list(s.cbegin(), s.cend(), 0, stats, global_scp);

  return 0;
}
