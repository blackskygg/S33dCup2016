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

  for (result.add_line(linum);
       result.add_line(linum), expr[1]->eval(new_scp);
       result.add_line(linum), expr[2]->eval(new_scp))
    {
      try { stat->execute(result, new_scp); }
      catch (BreakException e) { break; }
    }
}

void WhileStat::execute(Result& result, Scope& scope)
{
  while(result.add_line(linum), expr->eval(scope)) {
    try { stat->execute(result, scope); }
    catch (BreakException e) { break; }
  }
}

void DoStat::execute(Result& result, Scope& scope)
{
  do {
    try { stat->execute(result, scope); }
    catch (BreakException e) { break; }
  } while(result.add_line(linum), expr->eval(scope));
}

void BreakStat::execute(Result& result, Scope& scope)
{
  result.add_line(linum);
  throw BreakException();
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
    //this might throw an exception, but we leave it to the IterStats
    stat->execute(result, new_scp);
  }
}
