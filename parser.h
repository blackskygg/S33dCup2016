#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <utility>
#include <string>
#include <regex>
#include <memory>
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
  typedef enum{
    COMMA,
    ASSIGNMENT,
    EQUALITY,
    RELATIONAL,
    ADDITIVE,
    MULT,
    UNARY,
    POSTFIX,
    PRIMARY,
  } ExprType;

  virtual int eval(Scope &scope) = 0;
};

class AssignmentExpr: public Expression {
 public:
  AssignmentExpr() = default;
  AssignmentExpr (std::string id, std::shared_ptr<Expression> expr): id(id), expr(expr) {};
  int eval(Scope &scope);

  std::string id;
  std::shared_ptr<Expression> expr;
};

class PrimaryExprConst: public Expression {
 public:
  PrimaryExprConst() = default;
  PrimaryExprConst(int val): val(val) {};

  int eval(Scope &scope);
  int val;
};

class PrimaryExprId: public Expression {
 public:
 PrimaryExprId(std::string id): id(id) {};
  int eval(Scope &scope);
  
  std::string id;
};

class Statement {
 public:

  typedef enum {
    COMPOUND_STAT = 0,
    DECL_STAT = 1,
  }StatType;

  Statement() = default;
 Statement(Scope *scope): scope(scope) {};
  int execute() {};

  //begin and end are the positions of the begining and ending tokens
  size_t begin;
  size_t end;

  Scope *scope;
};

class CompoundStatement: public Statement {
 public:
  int execute();
  std::vector< std::shared_ptr<Statement> > stat_list;
};

class ExpressionStat : public Statement {
 public:
  std::shared_ptr<Expression> expr;
};

typedef std::vector<AssignmentExpr> InitDeclaratorList;
class DeclStat: public Statement {
 public:
 DeclStat(Scope *scope) {this->scope = scope;};
  int execute();
  InitDeclaratorList decl_list;
};


#define stat_regex(s) regex(s)
#define expr_regex(s) regex(s)

class Parser {
 public:
  Parser(std::vector <Token> &tokens);
  int parse(std::vector<Statement>& stats);
  
 private:
  void parse_expr(std::string::const_iterator begin,
		  std::string::const_iterator end,
		  size_t origin,
		  std::shared_ptr<Expression>& expr);
  void parse_assign_expr(std::string::const_iterator begin,
			 std::string::const_iterator end,
			 size_t origin,
			 AssignmentExpr& expr);
  void parse_decl_stat(std::string::const_iterator begin,
		       std::string::const_iterator end,
		       size_t origin,
		       DeclStat& stat);
  void parse_stats(std::string::const_iterator begin,
		   std::string::const_iterator end,
		   size_t origin,
		   std::vector<Statement>& stat,
		   std::vector<Token> &tokens, Scope *scope);
  void encode_tokens(std::vector <Token>& tokens, std::string &s);

  std::vector< std::pair<Statement::StatType, std::regex> > stat_tpls;
  std::vector< std::pair<Expression::ExprType, std::regex> > expr_tpls;
  char code_map[256];
  std::vector <Token> &tokens;
};


#endif
