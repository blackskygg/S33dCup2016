#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <utility>
#include <string>
#include <regex>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <exception>
#include "lexer.h"

using std::cout;
using std::endl;

/* A Scope represents the "scope" concept in C*/
class Scope {
 public:
 Scope() : parent(nullptr) {};
  Scope(Scope* parent) : parent(parent) {};
  long long get_identifier(const std::string& name);
  void add_identifier(const std::string& name, long long val);
  void mod_identifier(const std::string& name, long long val);

 private:
  Scope* parent = nullptr;
  std::unordered_map<std::string, long long> vars;
};

/* a Result holds the final answer */
class Result {
 public:
  void add_line(int linum);
  void print(std::ofstream& ofs);

 private:
  std::vector<int> lines;
  int prev = -1;
};

/* This is the base class for all Expression types.
 * Supported types are defined in Expression::ExprType.
 * We use long long as the value type to avoid overflow
 */
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

  virtual long long eval(Scope &scope)  = 0;
};

/* id = expr */
class AssignmentExpr: public Expression {
 public:
  long long eval(Scope &scope);
  
  std::string id;
  std::shared_ptr<Expression> expr;
};

/* expr, expr   where expr could be another CommaExpr */
class CommaExpr: public Expression {
 public:
  long long eval(Scope &scope);

  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

/* expr == expr */
class EqualityExpr: public Expression {
 public:
  long long eval(Scope &scope);

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

/* expr cmp_op expr */
class RelationalExpr: public Expression {
 public:
  long long eval(Scope &scope);

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

/* expr add/sub expr */
class AdditiveExpr: public Expression {
 public:
  long long eval(Scope &scope);

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

/* expr mult_op expr */
class MultExpr: public Expression {
 public:
  long long eval(Scope &scope);

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

/* +-expr */
class UnaryExpr: public Expression {
 public:
  long long eval(Scope &scope);

  std::string op; 
  std::shared_ptr<Expression> expr;
};

/* int const */
class PrimaryExprConst: public Expression {
 public:
  PrimaryExprConst(int val): val(val) {};
  long long eval(Scope &scope);

  long long val;
};

/* var_name */
class PrimaryExprId: public Expression {
 public:
 PrimaryExprId(std::string id): id(id) {};
  long long eval(Scope &scope);
  void print() {
    cout << "ID: " << id <<endl;
  };
  
  std::string id;
};

/* var++/-- */
class PostfixExpr: public Expression {
 public:
  long long eval(Scope &scope);
  
  std::string op; 
  std::shared_ptr<Expression> expr;
};

/* This is the base class for all the Statment types
 * Supported type are defined in  Statement::StatType
 */
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

  /* this exception is used to handle the "break" statement
   * we use this coz it's the most natural way
   */
  class BreakException : public std::exception {};

  Statement() = default;
  Statement(int linum): linum(linum) {};
  virtual void execute(Result& result, Scope& scope) {};

  int linum;
};

/* elements in the decl_list 
 * it's evaluation will add new items in the current scope
 */
class InitDecl: public Expression {
 public:
  long long eval(Scope &scope);

  std::string id;
  std::shared_ptr<Expression> expr;
};

/* declaration statement */
class DeclStat: public Statement {
 public:
  DeclStat(int linum): Statement(linum) {};
  void execute(Result &result, Scope& scope);

  bool has_init = false;
  std::vector<InitDecl> decl_list;
};

/* {stat_list} */
class CompoundStat: public Statement {
 public:
  CompoundStat(int linum): Statement(linum) {};
  void execute(Result &result, Scope& scope);

  std::vector< std::shared_ptr<Statement> > stat_list;
};

/* if .. else .. */
class SelectStat: public Statement {
 public:
  SelectStat(int linum): Statement(linum) {};
  void execute(Result &result, Scope& scope);

  bool has_else = false;
  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat1;
  std::shared_ptr<Statement> stat2;
};

/* for loop */
class ForStat: public Statement {
 public:
 ForStat(int linum):
  Statement(linum), decl(linum) {};
  void execute(Result &result, Scope& scope);

  bool has_decl = false;
  DeclStat decl;
  std::shared_ptr<Expression> expr[3];
  std::shared_ptr<Statement> stat;
};

/* while loop */
class WhileStat: public Statement {
 public:
  WhileStat(int linum) : Statement(linum) {};
  void execute(Result &result, Scope& scope);

  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat;
};

/* break statement, this will cause a BreakException */
class BreakStat: public Statement {
 public:
  BreakStat(int linum) : Statement(linum) {};
  void execute(Result &result, Scope& scope);
};

/* Do .. while loop */
class DoStat: public Statement {
 public:
  DoStat(int linum) : Statement(linum) {};
  void execute(Result &result, Scope& scope);

  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat;
};

/* print statement, argument list was treated as CommaExpr */
class PrintStat: public Statement {
 public:
  PrintStat(int linum) : Statement(linum){};
  void execute(Result &result, Scope& scope);

  std::shared_ptr<Expression> expr;
};

/* statements that only contains an expression */
class ExprStat : public Statement {
 public:
 ExprStat(int linum) : Statement(linum) {};
  void execute(Result &result, Scope& scope);

  bool is_empty = false;
  std::shared_ptr<Expression> expr;
};

/* the Parser parses the token list and calculate the final result */
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

/* below are parsers for specific types of statements 
 * the parsers' names are of the form  parse_(type)_stat();
 */
#define PARSER_NAME(fn)       parse_##fn##_stat
#define STAT_PARSER_PROTO(T, fn)				\
  std::string::const_iterator					\
    PARSER_NAME(fn)(std::string::const_iterator str_begin,	\
		      std::string::const_iterator str_end,	\
		      size_t origin,				\
		      T& stat)
  STAT_PARSER_PROTO(CompoundStat, comp);
  STAT_PARSER_PROTO(DeclStat, decl);
  STAT_PARSER_PROTO(SelectStat, select);
  STAT_PARSER_PROTO(ForStat, for);
  STAT_PARSER_PROTO(WhileStat, while);
  STAT_PARSER_PROTO(BreakStat, break);
  STAT_PARSER_PROTO(DoStat, do);
  STAT_PARSER_PROTO(PrintStat, print);
  STAT_PARSER_PROTO(ExprStat, expr);
#undef STAT_PARSER_PROTO


  std::string::const_iterator
    parse_stat(std::string::const_iterator begin,
	       std::string::const_iterator end,
	       size_t origin,
	       std::shared_ptr<Statement>& stats);

  std::string::const_iterator
    parse_stat_list(std::string::const_iterator begin,
		    std::string::const_iterator end,
		    size_t origin,
		    std::vector< std::shared_ptr<Statement> >& stats);
  
  void encode_tokens(std::vector <Token>& tokens, std::string &s);


  std::vector< std::pair<Expression::ExprType, std::regex> > expr_tpls;
  char code_map[256];
  std::vector <Token> &tokens;
};

#endif
