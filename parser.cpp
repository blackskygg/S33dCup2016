#include "parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int Scope::get_identifier(const string& name)
{
  unordered_map<string, int>::iterator it;
  if ((vars.end() == (it = vars.find(name))) && (parent != nullptr))
    return parent->get_identifier(name);
  else
    return it->second;
}

void Scope::mod_identifier(const string& name, int val)
{
  unordered_map<string, int>::iterator it;
  
  if ((vars.end() == (it = vars.find(name))) && (parent != nullptr))
    parent->mod_identifier(name, val);
  else
    vars[name] = val;
}

void Scope::set_identifier(const string& name, int val)
{
  vars[name] = val;
}

void Result::add_line(int linum) {
  cout<< linum << endl;
  if (linum != prev) {
    prev = linum;
    lines.push_back(linum);
  }
}

void Result::print()
{
  for(auto ln: lines)
    cout<<ln<<" ";
  cout<<endl;
}

int CommaExpr:: eval(Scope& scope)
{
  expr1->eval(scope);
  return expr2->eval(scope);
}

int EqualityExpr:: eval(Scope& scope)
{
  int val1 = expr1->eval(scope), val2 = expr2->eval(scope);
  int ret_val = 1;

  if ("!=" == op) ret_val = 1;
  return val1 == val2 ? ret_val : !ret_val;
}

int RelationalExpr:: eval(Scope& scope)
{
  int val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if (">=" == op) {
    return val1 >= val2 ? 1 : 0;
  } else if("<=" == op) {
    return val1 <= val2 ? 1 : 0;
  } else if(">" == op) {
    return val1 > val2 ? 1 : 0;
  } else if("<" == op) {
    return val1 < val2 ? 1 : 0;
  }
}

int AdditiveExpr:: eval(Scope& scope)
{
  int val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if ("+" == op) {
    return val1 + val2;
  } else if("-" == op) {
    return val1 - val2;
  }
}

int MultExpr:: eval(Scope& scope)
{
  int val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if ("*" == op) {
    return val1 * val2;
  } else if("/" == op) {
    return val1 / val2;
  }
}

int UnaryExpr:: eval(Scope& scope)
{
  int val = expr->eval(scope);

  if ("-" == op) {
    return -val;
  } else if("+" == op) {
    return val;
  }
}

int PostfixExpr:: eval(Scope& scope)
{
  int val = expr->eval(scope);
  string& id = dynamic_pointer_cast<PrimaryExprId>(expr)->id;
  if ("++" == op) {
    scope.mod_identifier(id, val + 1);
    return val + 1;
  } else if("--" == op) {
    scope.mod_identifier(id, val - 1);
    return val - 1;
  }
}

int PrimaryExprConst:: eval(Scope& scope)
{
  return val;
}

int PrimaryExprId:: eval(Scope& scope)
{
  return scope.get_identifier(id);
}

int AssignmentExpr::eval(Scope& scope)
{
  int val = expr->eval(scope);

  scope.mod_identifier(id, val);
  
  return val;
}

int InitDecl::eval(Scope& scope)
{
  int val = expr->eval(scope);

  scope.set_identifier(id, val);
  
  return val;
}

void DeclStat::execute(Result& result)
{
  if (has_init)
    result.add_line(linum);
  
  for (auto e: decl_list)
    e.eval(*scope);
}

void SelectStat::execute(Result& result)
{
  result.add_line(linum);
  if (expr->eval(*scope)) {
    stat1->execute(result);
  } else {
    stat2->execute(result);
  }
}

void ForStat::execute(Result& result)
{
  if (has_decl)
    decl.execute(result);
  else
    expr[1]->eval(*scope);

  for (result.add_line(linum);
       result.add_line(linum), expr[2]->eval(*scope);
       expr[3]->eval(*scope)) {
    stat->execute(result);
  }
}

void WhileStat::execute(Result& result)
{
  while(result.add_line(linum), expr->eval(*scope)) {
    stat->execute(result);
  }
}

void DoStat::execute(Result& result)
{
  do {
    stat->execute(result);
    result.add_line(linum);
  } while(expr->eval(*scope));
}

void BreakStat::execute(Result& result)
{
  result.add_line(linum);
}

void PrintStat::execute(Result& result)
{
  result.add_line(linum);
  expr->eval(*scope);
}

void ExprStat::execute(Result& result)
{
  if (!is_empty) {
    result.add_line(linum);
    expr->eval(*scope);
  }
}

void CompoundStat::execute(Result& result)
{
  for (auto stat: stat_list)
    stat->execute(result);
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
  expr_tpls.push_back(make_pair(Expression::ADDITIVE, expr_regex("(.*[\\^vnas])([\\+-])(.+)")));
  expr_tpls.push_back(make_pair(Expression::MULT, expr_regex("(.+)([\\*/])(.+)")));
  expr_tpls.push_back(make_pair(Expression::UNARY, expr_regex("[\\+-](.+)")));
  expr_tpls.push_back(make_pair(Expression::POSTFIX, expr_regex("(.+)([\\^v])")));
  expr_tpls.push_back(make_pair(Expression::PRIMARY, expr_regex("([nas])")));
}

void Parser::encode_tokens(std::vector <Token>& tokens, string &s)
{
  for (auto token: tokens) 
    s.push_back(code_map[token.type]);
}


void Parser::parse(Result& result)
{
  string s;
  shared_ptr<Scope> global_scp = make_shared<Scope>();
  CompoundStat stat(global_scp, 1);
  
  encode_tokens(tokens, s);

  parse_stat_list(s.cbegin(), s.cend(), 0, stat.stat_list, global_scp);
  stat.execute(result);
}