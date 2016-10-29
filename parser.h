#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <utility>
#include <string>
#include <regex>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include "lexer.h"

using namespace std;

class Identifier {
 public:
  std::string name;
  int val;
};

class Scope {
 public:
 Scope() : parent(nullptr), is_top(true){};
  Scope(Scope* parent) : parent(parent), is_top(false) {};
  int get_identifier(const std::string& name);
  void set_identifier(const std::string& name, int val);
  void mod_identifier(const std::string& name, int val);

 private:
  bool is_top = true;
  Scope* parent = nullptr;
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
  virtual void print() {

  };

};

class AssignmentExpr: public Expression {
 public:
  int eval(Scope &scope);
  void print() {
    cout << "ASSIGN: " <<endl;
    cout << "ID: " << this->id <<endl;
    cout << "Expr: ";
    expr->print();
  };

  std::string id;
  std::shared_ptr<Expression> expr;
};

class CommaExpr: public Expression {
 public:
  int eval(Scope &scope);
  void print() {
    cout << "COMMA: "<< endl;
    cout << "Expr1: ";
    expr1->print();
    cout<<endl << "Expr2: ";
    expr2->print();
  };

  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

class EqualityExpr: public Expression {
 public:
  int eval(Scope &scope);
  void print() {
    cout << "OP: " << op <<endl;
    cout << "Expr1: ";
    expr1->print();
    cout << "Expr2: ";
    expr2->print();
  };

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

class RelationalExpr: public Expression {
 public:
  int eval(Scope &scope);
  void print() {
    cout << "OP: " << op <<endl;
    cout << "Expr1: ";
    expr1->print();
    cout <<"Expr2: ";
    expr2->print();
  };

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

class AdditiveExpr: public Expression {
 public:
  int eval(Scope &scope);
  void print() {
    cout << "OP: " << op <<endl;
    cout << "Expr1: ";
    expr1->print();
    cout << "Expr2: ";
    expr2->print();
  };

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

class MultExpr: public Expression {
 public:
  int eval(Scope &scope);
  void print() {
    cout << "OP: " << op <<endl;
    cout << "Expr1: ";
    expr1->print();
    cout << "Expr2: ";
    expr2->print();
  };

  std::string op;
  std::shared_ptr<Expression> expr1;
  std::shared_ptr<Expression> expr2;
};

class UnaryExpr: public Expression {
 public:
  int eval(Scope &scope);
  void print() {
    cout << "UNARYOP: " << op <<endl;
    cout << "Expr: ";
    expr->print();
  };

  std::string op; 
  std::shared_ptr<Expression> expr;
};

class PrimaryExprConst: public Expression {
 public:
  PrimaryExprConst(int val): val(val) {};
  int eval(Scope &scope);
  void print() {
    cout << "const: " << val <<endl;
  };

  int val;
};

class PrimaryExprId: public Expression {
 public:
 PrimaryExprId(std::string id): id(id) {};
  int eval(Scope &scope);
  void print() {
    cout << "ID: " << id <<endl;
  };
  
  std::string id;
};

class PostfixExpr: public Expression {
 public:
  int eval(Scope &scope);
  void print() {
    cout << "POSTOP: " << op <<endl;
    cout << "Expr: ";
    expr->print();
  };
  
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
  Statement(int linum): linum(linum) {};
  virtual void execute(Result& result, Scope& scope) {};
  virtual void print() {
  };

  int linum;
};

class InitDecl: public Expression {
 public:
  int eval(Scope &scope);
  void print() {
    cout << "InitDecl: " <<  id <<endl;
    cout << "ID: " <<  id <<endl;
    cout << "Expr: ";
    expr->print();
  };

  std::string id;
  std::shared_ptr<Expression> expr;
};

class DeclStat: public Statement {
 public:
  DeclStat(int linum): Statement(linum) {};
  void execute(Result &result, Scope& scope);
  void print() {
    cout << "Decl: " << endl;
    for(auto decl: this->decl_list) {
      decl.print();
    }
    cout<<endl<<"DeclEnd"<<endl;
  };

  bool has_init = false;
  std::vector<InitDecl> decl_list;
};

class CompoundStat: public Statement {
 public:
  CompoundStat(int linum): Statement(linum) {};
  void execute(Result &result, Scope& scope);
  void print() {
    cout << "Compound: " << endl;
    for(auto stat: this->stat_list) {
      stat->print();
    }
    cout<<endl<<"CompoundEnd"<<endl;
  };
  
  std::vector< std::shared_ptr<Statement> > stat_list;
};

class SelectStat: public Statement {
 public:
  SelectStat(int linum): Statement(linum) {};
  void execute(Result &result, Scope& scope);
  void print() {
    cout << "If: " << endl;
    expr->print();
    stat1->print();
    if (this->has_else) {
      stat2->print();
    }
    cout<<endl<<"IfEnd"<<endl;
  };

  bool has_else;
  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat1;
  std::shared_ptr<Statement> stat2;
};

class ForStat: public Statement {
 public:
 ForStat(int linum):
  Statement(linum), decl(linum) {};
  void execute(Result &result, Scope& scope);
  void print() {
    cout << "For: " << endl;
    if (has_decl)
      decl.print();
    else
      expr[0]->print();
    cout<<"ForExpr1"<<endl;
    expr[1]->print();
    cout<<"ForExpr2"<<endl;
    expr[2]->print();
    cout<<"ForStat2"<<endl;
    stat->print();
    cout<<endl<<"ForEnd"<<endl;
  };

  bool has_decl = false;
  DeclStat decl;
  std::shared_ptr<Expression> expr[3];
  std::shared_ptr<Statement> stat;
};

class WhileStat: public Statement {
 public:
  WhileStat(int linum) : Statement(linum) {};
  void execute(Result &result, Scope& scope);
  void print() {
    cout << "While: " << endl;
    expr->print();
    stat->print();
    cout<<endl<<"WhileEnd"<<endl;
  };

  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat;
};

class BreakStat: public Statement {
 public:
  BreakStat(int linum) : Statement(linum) {};
  void execute(Result &result, Scope& scope);
  void print() {
    cout << "Break: " << endl;
    cout << "BreakEnd: "<<endl;
  };
};

class DoStat: public Statement {
 public:
  DoStat(int linum) : Statement(linum) {};
  void execute(Result &result, Scope& scope);
  void print() {
    cout << "Do: " << endl;
    expr->print();
    stat->print();
    cout<<endl<<"DoEnd"<<endl;
  };

  std::shared_ptr<Expression> expr;
  std::shared_ptr<Statement> stat;
};

class PrintStat: public Statement {
 public:
  PrintStat(int linum) : Statement(linum){};
  void execute(Result &result, Scope& scope);
  void print() {
    cout << "Print: " << endl;
    expr->print();
    cout<<endl<<"PrintEnd"<<endl;
  };

  std::shared_ptr<Expression> expr;
};

class ExprStat : public Statement {
 public:
 ExprStat(int linum) : Statement(linum) {};
  void execute(Result &result, Scope& scope);
  void print() {
    cout << "ExprStat: " << endl;
    expr->print();
    cout<<endl<<"ExprStatEnd"<<endl;
  };

  bool is_empty = false;
  std::shared_ptr<Expression> expr;
};


#define stat_regex(s) regex(s)
#define expr_regex(s) regex(s)

#define PARSER_NAME(fn)       parse_##fn##_stat
#define STAT_PARSER_PROTO(T, fn)				\
  std::string::const_iterator					\
    PARSER_NAME(fn)(std::string::const_iterator str_begin,	\
		      std::string::const_iterator str_end,	\
		      size_t origin,				\
		      T& stat)

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

  STAT_PARSER_PROTO(CompoundStat, comp);
  STAT_PARSER_PROTO(DeclStat, decl);
  STAT_PARSER_PROTO(SelectStat, select);
  STAT_PARSER_PROTO(ForStat, for);
  STAT_PARSER_PROTO(WhileStat, while);
  STAT_PARSER_PROTO(BreakStat, break);
  STAT_PARSER_PROTO(DoStat, do);
  STAT_PARSER_PROTO(PrintStat, print);
  STAT_PARSER_PROTO(ExprStat, expr);

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


  std::vector< std::pair<Statement::StatType, std::regex> > stat_tpls;
  std::vector< std::pair<Expression::ExprType, std::regex> > expr_tpls;
  char code_map[256];
  std::vector <Token> &tokens;

};

#endif
