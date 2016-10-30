#include "parser.h"

using namespace std;

void DeclStat::execute(Result& result, Scope& scope)
{
  if (has_init) result.add_line(linum);
  
  for (auto e: decl_list)
    e.eval(scope);
}

void SelectStat::execute(Result& result, Scope& scope)
{
  result.add_line(linum);
  if (expr->eval(scope)) stat1->execute(result, scope);
  else if (has_else) stat2->execute(result, scope);
}

void ForStat::execute(Result& result, Scope& scope)
{
  Scope new_scp(&scope);
  
  if (has_decl) decl.execute(result, new_scp);
  else expr[0]->eval(new_scp);

  result.add_line(linum);
  while (result.add_line(linum), expr[1]->eval(new_scp)) {
    stat->execute(result, new_scp);
    if (new_scp.break_flag) break;
    
    result.add_line(linum), expr[2]->eval(new_scp);
  }
}

void WhileStat::execute(Result& result, Scope& scope)
{
  while(result.add_line(linum), expr->eval(scope)) {
    stat->execute(result, scope);
    if (scope.break_flag) {
      scope.break_flag = false;
      break;
    }
  }
}

void DoStat::execute(Result& result, Scope& scope)
{
  do {
    stat->execute(result, scope);
    if (scope.break_flag) {
      scope.break_flag = false;
      break;
    }
  } while(result.add_line(linum), expr->eval(scope));
}

void BreakStat::execute(Result& result, Scope& scope)
{
  scope.break_flag = true;
  result.add_line(linum);
}

void PrintStat::execute(Result& result, Scope& scope)
{
  result.add_line(linum);
  expr->eval(scope);
}

void ExprStat::execute(Result& result, Scope& scope)
{
  if (!is_empty) {
    result.add_line(linum);
    expr->eval(scope);
  }
}

void CompoundStat::execute(Result& result, Scope& scope)
{
  Scope new_scp(&scope);
  for (auto stat: stat_list) {
    stat->execute(result, new_scp);
    if (new_scp.break_flag) {
      scope.break_flag = true;
      break;
    }
  }
}
