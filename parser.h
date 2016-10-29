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
  int get_identifier(const std::string& name);
  void set_identifier(const std::string& name, int val);
  void mod_identifier(const std::string& name, int val);

 private:
  std::shared_ptr<Scope> parent;
  std::unordered_map<std::string, int> vars;
};

class Result {
 public:
  void add_line(int linum);
  void print();

 private:
  std::vector<int> lines;
  int prev = -1;
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

  virtual int eval(Scope &scope)  = 0;
};

class AssignmentExpr: public Expression {
 public:
  int eval(Scope &scope);

  std::string id;
  std::shared_ptr<Expression> expr;
};

class CommaExpr: public Expression {
 public:
  int eval(Scope &scope);

  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

class EqualityExpr: public Expression {
 public:
  int eval(Scope &scope);

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

class RelationalExpr: public Expression {
 public:
  int eval(Scope &scope);

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

class AdditiveExpr: public Expression {
 public:
  int eval(Scope &scope);

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

class MultExpr: public Expression {
 public:
  int eval(Scope &scope);

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

class UnaryExpr: public Expression {
 public:
  int eval(Scope &scope);

  std::string op; 
  std::shared_ptr<Expression> expr;
};

class PrimaryExprConst: public Expression {
 public:
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

class PostfixExpr: public Expression {
 public:
  int eval(Scope &scope);
  
  std::string op; 
  std::shared_ptr<Expression> expr;
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
  virtual void execute(Result& lines) = 0;

  std::shared_ptr<Scope> scope;
  int linum;
};

class InitDecl: public Expression {
 public:
  int eval(Scope &scope);

  std::string id;
  std::shared_ptr<Expression> expr;
};

class DeclStat: public Statement {
 public:
  DeclStat(std::shared_ptr<Scope> scope, int linum): Statement(scope, linum) {};
  void execute(Result &result);

  bool has_init = false;
  std::vector<InitDecl> decl_list;
};

class CompoundStat: public Statement {
 public:
  CompoundStat(std::shared_ptr<Scope> scope, int linum): Statement(scope, linum) {};
  void execute(Result &result);
  
  std::vector< std::shared_ptr<Statement> > stat_list;
};

class SelectStat: public Statement {
 public:
  SelectStat(std::shared_ptr<Scope> scope, int linum): Statement(scope, linum) {};
  void execute(Result &result);

  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat1;
  std::shared_ptr<Statement> stat2;
};

class ForStat: public Statement {
 public:
 ForStat(std::shared_ptr<Scope> scope, int linum):
  Statement(scope, linum), decl(scope, linum) {};
  void execute(Result &result);

  bool has_decl = false;
  DeclStat decl;
  std::shared_ptr<Expression> expr[3];
  std::shared_ptr<Statement> stat;
};

class WhileStat: public Statement {
 public:
  WhileStat(std::shared_ptr<Scope> scope, int linum) : Statement(scope, linum) {};
  void execute(Result &result);

  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat;
};

class BreakStat: public Statement {
 public:
  BreakStat(std::shared_ptr<Scope> scope, int linum) : Statement(scope, linum) {};
  void execute(Result &result);
};

class DoStat: public Statement {
 public:
  DoStat(std::shared_ptr<Scope> scope, int linum) : Statement(scope, linum) {};
  void execute(Result &result);

  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat;
};

class PrintStat: public Statement {
 public:
  PrintStat(std::shared_ptr<Scope> scope, int linum) : Statement(scope, linum){};
  void execute(Result &result);

  std::shared_ptr<Expression> expr;
};

class ExprStat : public Statement {
 public:
 ExprStat(std::shared_ptr<Scope> scope, int linum) : Statement(scope, linum) {};
  void execute(Result &result);
  
  std::shared_ptr<Expression> expr;
};


#define stat_regex(s) regex(s)
#define expr_regex(s) regex(s)

class Parser {
 public:
  Parser(std::vector <Token> &tokens);
  void parse(Result& result);
  
 private:
  void parse_expr(std::string::const_iterator begin,
		  std::string::const_iterator end,
		  size_t origin,
		  std::shared_ptr<Expression>& expr);
  void parse_init_decl(std::string::const_iterator begin,
			 std::string::const_iterator end,
			 size_t origin,
			 InitDecl& decl);
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
  std::string::const_iterator parse_break_stat(std::string::const_iterator str_begin,
						  std::string::const_iterator str_end,
						  size_t origin,
						  BreakStat& stat);
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
