#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <utility>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "lexer.h"

class Identifier {
 public:
  std::string name;
  int val;
};

class Scope {
 public:
  Identifier& get_identifier(std::string name);
};

class Expression{
 public:
  Scope *scope;
  int eval() {};
};

class AssignmentExpr: Expression {
 public:
  AssignmentExpr (std::string id, Expression expr): id(id), expr(expr) {};
  int eval();
  
  std::string id;
  Expression expr;
};

class PrimaryExpr: Expression {
 public:
  PrimaryExpr(int val): val(val) {};
  int eval();
  
  int val;
};

class Statement {
 public:
  int execute() {};
};

class CompoundStatement: Statement {
 public:
  int execute();

  std::vector< Statement > stat_list;
};

class ExpressionStat : Statement {
 public:
  ExpressionStat(Expression expr): expr(expr) {};

  Expression expr;
};

typedef std::vector<AssignmentExpr> InitDeclaratorList;
class DeclStat: Statement {
 public:
  int execute();
  InitDeclaratorList init_decl_list;
};

class Parser {
 public:
  Parser();
  int parse(std::vector <Token>& tokens, std::vector<Statement>& stats);
 private:
  int encode_tokens(std::vector <Token>& tokens, std::string &s);
  char code_map[256];
};


#endif
