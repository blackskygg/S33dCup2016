#include "parser.h"
#include <iostream>

using namespace std;

Identifier& Scope::get_identifier(string name)
{
}

int PrimaryExpr:: eval()
{
  return val;
}

int AssignmentExpr::eval()
{
  return scope->get_identifier(id).val = expr.eval();
}

int DeclStat::execute()
{
  for (auto decl: init_decl_list)
    decl.eval();

  return 0;
}

int CompoundStatement::execute()
{
  for (auto stat: stat_list)
    stat.execute();
   
  return 0;
}

Parser::Parser()
{
  //  code_map[BLANK] = '';
  code_map[INT_TYPE] = 'i';
  code_map[FOR] = 'f';
  code_map[IF] = '?';
  code_map[ELSE] = '@';
  code_map[DO] = 'd';
  code_map[WHILE] = 'w';
  code_map[BREAK] = 'b';
  code_map[PRINTF] = 'p';
  code_map[IDENTIFIER] = 'a';
  code_map[NUMBER] = 'n';
  code_map[RP] = ')';
  code_map[LP] = '(';
  code_map[LB] = '{';
  code_map[RB] = '}';
  code_map[ADD] = '+';
  code_map[SUB] = '-';
  code_map[MUL] = '*';
  code_map[DIV] = '/';
  code_map[DEC] = 'v';
  code_map[INC] = '^';
  code_map[ASSIGNMENT] = '=';
  code_map[NE] = '!';
  code_map[GE] = 'g';
  code_map[LE] = 'l';
  code_map[GT] = '>';
  code_map[LT] = '<';
  code_map[EQ] = 'e';
  code_map[COLON] = ':';
  code_map[COMMA] = ',';
  code_map[SEMICOLON] = ';';
  //  code_map[COMMENT] = '';
  //  code_map[CRLF] = '';
  code_map[STRING_LITERAL] = 's';
}

int Parser::encode_tokens(std::vector <Token>& tokens, string &s)
{
  for (auto token: tokens) 
    s.push_back(code_map[token.type]);
  return 0;
}

int Parser::parse(vector <Token>& tokens, vector<Statement>& stats)
{
  string s;
  
  encode_tokens(tokens, s);

  cout<<s;
  
  return 0;
}
