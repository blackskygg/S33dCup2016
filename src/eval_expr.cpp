#include "parser.h"

using namespace std;

int CommaExpr:: eval(Scope& scope)
{
  return expr1->eval(scope), expr2->eval(scope);
}

int EqualityExpr:: eval(Scope& scope)
{
  int val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if ("!=" == op) return val1 != val2;
  return val1 == val2;
}

int RelationalExpr:: eval(Scope& scope)
{
  int val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if (">=" == op) return val1 >= val2;
  else if("<=" == op) return val1 <= val2;
  else if(">" == op) return val1 > val2;
  else if("<" == op) return val1 < val2;
  else return -1;
}

int AdditiveExpr:: eval(Scope& scope)
{
  int val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if ("+" == op) return val1 + val2;
  else if("-" == op) return val1 - val2;
  else return -1;
}

int MultExpr:: eval(Scope& scope)
{
  int val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if ("*" == op) return val1 * val2;
  else if("/" == op) return val1 / val2;
  else return -1;
}

int UnaryExpr:: eval(Scope& scope)
{
  int val = expr->eval(scope);

  if ("-" == op) return -val;
  else if("+" == op) return val;
  else return -1;
}

int PostfixExpr:: eval(Scope& scope)
{
  int val = expr->eval(scope);
  string& id = dynamic_pointer_cast<PrimaryExprId>(expr)->id;

  if ("++" == op) scope.mod_identifier(id, val + 1);
  else if("--" == op) scope.mod_identifier(id, val - 1);

  return val;
}

int PrimaryExprConst:: eval(Scope& scope) {return val;}

int PrimaryExprId:: eval(Scope& scope) {return scope.get_identifier(id);}

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
