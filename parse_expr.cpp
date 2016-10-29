#include "parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void Parser::parse_expr(string::const_iterator str_begin,
			string::const_iterator str_end,
			size_t origin,
			shared_ptr<Expression>& expr)
{
  smatch m;
  Expression::ExprType type = Expression::PRIMARY;

  for (auto tpl: expr_tpls) {
    if (regex_search(str_begin, str_end, m, tpl.second,
		     regex_constants::match_continuous)) {
      type = tpl.first;
      break;
    }
  }

#define POS(n)	(m[(n)].first - str_begin + origin)
#define PARSE_EXPR(n, expr)						\
  parse_expr(m[(n)].first, m[(n)].second, POS(n), expr);
#define PARSE_BIN(var)			\
  var->op = tokens[POS(2)].code;	\
  PARSE_EXPR(1, var->expr1);		\
  PARSE_EXPR(3, var->expr2);		

  //  cout<< "op: " << var->op <<endl;	

  //  cout << "expr: " << string(str_begin, str_end) << endl;
  //  cout << "expr_type: " << type << endl;
  switch (type) {
  case Expression::COMMA:
    {
      shared_ptr<CommaExpr> comma_ptr = make_shared<CommaExpr>();
      PARSE_EXPR(1, comma_ptr->expr1);
      PARSE_EXPR(2, comma_ptr->expr2);
      expr = dynamic_pointer_cast<Expression>(comma_ptr);
    }
    break;
  case Expression::ASSIGNMENT:
    {
      shared_ptr<AssignmentExpr> assign_ptr = make_shared<AssignmentExpr>();
      assign_ptr->id = tokens[origin].code;
      PARSE_EXPR(2, assign_ptr->expr);
      expr = dynamic_pointer_cast<Expression>(assign_ptr);
    }
    break;
  case Expression::EQUALITY:
    {
      shared_ptr<EqualityExpr> equality_ptr = make_shared<EqualityExpr>();
      PARSE_BIN(equality_ptr);
      expr = dynamic_pointer_cast<Expression>(equality_ptr);
    }
    break;
  case Expression::RELATIONAL:
    {
      shared_ptr<RelationalExpr> relation_ptr = make_shared<RelationalExpr>();
      PARSE_BIN(relation_ptr);
      expr = dynamic_pointer_cast<Expression>(relation_ptr);
    }
    break;
  case Expression::ADDITIVE:
    {
      shared_ptr<AdditiveExpr> add_ptr = make_shared<AdditiveExpr>();
      PARSE_BIN(add_ptr);
      expr = dynamic_pointer_cast<Expression>(add_ptr);
    }
    break;
  case Expression::MULT:
    {
      shared_ptr<MultExpr> mul_ptr = make_shared<MultExpr>();
      PARSE_BIN(mul_ptr);
      expr = dynamic_pointer_cast<Expression>(mul_ptr);
    }
    break;
  case Expression::UNARY:
    {
      shared_ptr<UnaryExpr> unary_ptr = make_shared<UnaryExpr>();
      unary_ptr->op = tokens[POS(1)].code;
      PARSE_EXPR(2, unary_ptr->expr);
      expr = dynamic_pointer_cast<Expression>(unary_ptr);
    }
    break;
  case Expression::POSTFIX:
    {
      shared_ptr<PostfixExpr> post_ptr  = make_shared<PostfixExpr>();
      PARSE_EXPR(1, post_ptr->expr);
      post_ptr->op = tokens[POS(2)].code;
      expr = dynamic_pointer_cast<Expression>(post_ptr);
    }
    break;
  case Expression::PRIMARY:
    if (m[0].str() == "n") {
      shared_ptr<PrimaryExprConst> prim_ptr = make_shared<PrimaryExprConst> \
	(atoi(tokens[POS(1)].code.c_str()));
      expr = dynamic_pointer_cast<Expression>(prim_ptr);
    } else  if (m[0].str() == "s") {
      shared_ptr<PrimaryExprConst> prim_ptr = make_shared<PrimaryExprConst>(0);
      expr = dynamic_pointer_cast<Expression>(prim_ptr);
    } else if (m[1].str() == "a"){
      shared_ptr<PrimaryExprId> prim_ptr = make_shared<PrimaryExprId>	\
	(tokens[POS(1)].code);
      expr = dynamic_pointer_cast<Expression>(prim_ptr);
    } else {
      shared_ptr<PrimaryExprConst> prim_ptr = make_shared<PrimaryExprConst>(0);
      expr = dynamic_pointer_cast<Expression>(prim_ptr);
    }
    break;
  default:
    {
      shared_ptr<PrimaryExprConst> prim_ptr = make_shared<PrimaryExprConst>(0);
      expr = dynamic_pointer_cast<Expression>(prim_ptr);
      cout <<" Expression reached default "<<endl;
    }
    break;
  }

#undef PARSE_BIN    
#undef PARSE_EXPR
#undef POS
}
