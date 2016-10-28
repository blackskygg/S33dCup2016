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
  Scope() {parent = nullptr;}; 
  Scope(std::shared_ptr<Scope> parent) : parent(parent) {};
  Identifier& get_identifier(std::string name);

 private:
  std::shared_ptr<Scope> parent;
  std::unordered_map<std::string, int> vars;
};

class Expression{
 public:
  typedef enum{
    COMMA = 0,
    ASSIGNMENT = 1,
    EQUALITY = 2,
    RELATIONAL = 3,
    ADDITIVE = 4,
    MULT = 5,
    UNARY = 6,
    POSTFIX = 7,
    PRIMARY = 8,
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
    DECL_STAT = 0,
    COMPOUND_STAT = 1,
    SELECTION_STAT = 2,
    ITERATION_STAT_DO = 3,
    ITERATION_STAT_WHILE = 4,
    ITERATION_STAT_FOR = 5,
    JUMP_STAT = 6,
    PRINT_STAT = 7,
  }StatType;

  Statement() = default;
  Statement(std::shared_ptr<Scope> scope, int linum): scope(scope), linum(linum) {};
  int execute() {};

  std::shared_ptr<Scope> scope;
  int linum;
};

class DeclStat: public Statement {
 public:
  DeclStat(std::shared_ptr<Scope> scope, int linum): Statement(scope, linum) {};
  int execute();

  std::vector<AssignmentExpr> decl_list;
};

class CompoundStat: public Statement {
 public:
  CompoundStat(std::shared_ptr<Scope> scope, int linum): Statement(scope, linum) {};
  int execute();
  
  std::vector< std::shared_ptr<Statement> > stat_list;
};

class SelectStat: public Statement {
 public:
  SelectStat(std::shared_ptr<Scope> scope, int linum): Statement(scope, linum) {};
  int execute();

  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat1;
  std::shared_ptr<Statement> stat2;
};

class ForStat: public Statement {
 public:
  ForStat(std::shared_ptr<Scope> scope, int linum): Statement(scope, linum) {};
  int execute();

  std::shared_ptr<Expression> expr[3];
  std::shared_ptr<Statement> stat;
};

class WhileStat: public Statement {
 public:
  WhileStat(std::shared_ptr<Scope> scope, int linum) : Statement(scope, linum) {};
  int execute();

  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat;
};

class DoStat: public Statement {
 public:
  DoStat(std::shared_ptr<Scope> scope, int linum) : Statement(scope, linum) {};
  int execute();

  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat;
};

class PrintStat: public Statement {
 public:
  PrintStat(std::shared_ptr<Scope> scope, int linum) : Statement(scope, linum){};
  int execute();

  std::shared_ptr<Expression> expr;
};

class ExprStat : public Statement {
 public:
 ExprStat(std::shared_ptr<Scope> scope, int linum) : Statement(scope, linum) {};
  int execute();
  
  std::shared_ptr<Expression> expr;
};


#define stat_regex(s) regex(s)
#define expr_regex(s) regex(s)

class Parser {
 public:
  Parser(std::vector <Token> &tokens);
  int parse(std::vector<std::shared_ptr<Statement> >& stats);
  
 private:
  void parse_expr(std::string::const_iterator begin,
		  std::string::const_iterator end,
		  size_t origin,
		  std::shared_ptr<Expression>& expr);
  void parse_assign_expr(std::string::const_iterator begin,
			 std::string::const_iterator end,
			 size_t origin,
			 AssignmentExpr& expr);
  void parse_stat_list(std::string::const_iterator begin,
		   std::string::const_iterator end,
		   size_t origin,
		   std::vector< std::shared_ptr<Statement> >& stats,
		   std::shared_ptr<Scope> scope);
  std::string::const_iterator parse_comp_stat(std::string::const_iterator str_begin,
					      std::string::const_iterator str_end,
					      size_t origin,
					      CompoundStat& stat);
  std::string::const_iterator parse_decl_stat(std::string::const_iterator begin,
					      std::string::const_iterator end,
					      size_t origin,
					      DeclStat& stat);
  std::string::const_iterator parse_select_stat(std::string::const_iterator str_begin,
						  std::string::const_iterator str_end,
						  size_t origin,
						  SelectStat& stat);
  std::string::const_iterator parse_for_stat(std::string::const_iterator str_begin,
						  std::string::const_iterator str_end,
						  size_t origin,
						  ForStat& stat);
  std::string::const_iterator parse_while_stat(std::string::const_iterator str_begin,
						  std::string::const_iterator str_end,
						  size_t origin,
						  WhileStat& stat);
  std::string::const_iterator parse_do_stat(std::string::const_iterator str_begin,
						  std::string::const_iterator str_end,
						  size_t origin,
						  DoStat& stat);
  std::string::const_iterator parse_print_stat(std::string::const_iterator str_begin,
						 std::string::const_iterator str_end,
						 size_t origin,
						 PrintStat& stat);
  std::string::const_iterator parse_expr_stat(std::string::const_iterator str_begin,
						std::string::const_iterator str_end,
						size_t origin,
						ExprStat& stat); 
  std::string::const_iterator parse_stat(std::string::const_iterator begin,
					 std::string::const_iterator end,
					 size_t origin,
					 std::shared_ptr<Statement>& stats,
					 std::shared_ptr<Scope> scope);
  
  void encode_tokens(std::vector <Token>& tokens, std::string &s);

  std::vector< std::pair<Statement::StatType, std::regex> > stat_tpls;
  std::vector< std::pair<Expression::ExprType, std::regex> > expr_tpls;
  char code_map[256];
  std::vector <Token> &tokens;
};


#endif
