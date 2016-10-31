#include "parser.h"

using namespace std;

long long CommaExpr:: eval(Scope& scope)
{
  return expr1->eval(scope), expr2->eval(scope);
}

long long EqualityExpr:: eval(Scope& scope)
{
  long long val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if ("!=" == op) return val1 != val2;
  return val1 == val2;
}

long long RelationalExpr:: eval(Scope& scope)
{
  long long val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if (">=" == op) return val1 >= val2;
  else if("<=" == op) return val1 <= val2;
  else if(">" == op) return val1 > val2;
  else if("<" == op) return val1 < val2;
  else return -1;
}

long long AdditiveExpr:: eval(Scope& scope)
{
  long long val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if ("+" == op) return val1 + val2;
  else if("-" == op) return val1 - val2;
  else return -1;
}

long long MultExpr:: eval(Scope& scope)
{
  long long val1 = expr1->eval(scope), val2 = expr2->eval(scope);

  if ("*" == op) return val1 * val2;
  else if("/" == op) return val1 / val2;
  else return -1;
}

long long UnaryExpr:: eval(Scope& scope)
{
  long long val = expr->eval(scope);

  if ("-" == op) return -val;
  else if("+" == op) return val;
  else return -1;
}

long long PostfixExpr:: eval(Scope& scope)
{
  long long val = expr->eval(scope);
  string& id = dynamic_pointer_cast<PrimaryExprId>(expr)->id;

  if ("++" == op) scope.mod_identifier(id, val + 1);
  else if("--" == op) scope.mod_identifier(id, val - 1);

  return val;
}

long long PrimaryExprConst:: eval(Scope& scope) {return val;}

long long PrimaryExprId:: eval(Scope& scope) {return scope.get_identifier(id);}

long long AssignmentExpr::eval(Scope& scope)
{
  long long val = expr->eval(scope);

  scope.mod_identifier(id, val);
  
  return val;
}

long long InitDecl::eval(Scope& scope)
{
  long long val = expr->eval(scope);

  scope.add_identifier(id, val);
  
  return val;
}
