#include "parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void Parser::parse_assign_expr(string::const_iterator str_begin,
			       string::const_iterator str_end,
			       size_t origin,
			       AssignmentExpr& expr) 
{
  smatch m;
  expr.id = tokens[origin].code;
  parse_expr(str_begin+2, str_end, origin + 2, expr.expr);
}

void Parser::parse_expr(string::const_iterator str_begin,
			string::const_iterator str_end,
			size_t origin,
			shared_ptr<Expression>& expr)
{
  smatch m;
  Expression::ExprType type;
  size_t begin_pos, end_pos;
  auto str_origin = str_begin;

  for ( ; str_begin != str_end; str_begin += m[0].length()) {
    //search for best match
    for (auto tpl: expr_tpls) {
      if (regex_search(str_begin, str_end, m, tpl.second,
		       regex_constants::match_continuous)) {
	type = tpl.first;
	break;
      }
    }

    if (!m.size())
      break;

    begin_pos = str_begin - str_origin + origin;
    end_pos = begin_pos + m[0].length();

    cout << "expr_type: " << type << endl;
    switch (type) {
    case Expression::COMMA:
      break;
    case Expression::ASSIGNMENT:
      break;
    case Expression::EQUALITY:
      break;
    case Expression::RELATIONAL:
      break;
    case Expression::ADDITIVE:
      break;
    case Expression::MULT:
      break;
    case Expression::UNARY:
      break;
    case Expression::POSTFIX:
      break;
    case Expression::PRIMARY:
      if (m[0].str() == "n") {
	expr = dynamic_pointer_cast<Expression>				\
	  (make_shared<PrimaryExprConst>				\
	   (atoi(tokens[begin_pos].code.c_str())));
      } else  if (m[0].str() == "n") {
	expr = dynamic_pointer_cast<Expression>				\
	  (make_shared<PrimaryExprConst>(0));
      } else {
	expr = dynamic_pointer_cast<Expression>				\
	  (make_shared<PrimaryExprId>					\
	   (tokens[begin_pos].code));
      }
      break;
    default:
      break;
    }
    
  }
  
}
