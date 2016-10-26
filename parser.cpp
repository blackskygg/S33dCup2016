#include "parser.h"
#include <iostream>

using namespace std;

Identifier& Scope::get_identifier(string name)
{
}

int PrimaryExpr:: eval()
{
  return val;
}

int AssignmentExpr::eval()
{
  return scope->get_identifier(id).val = expr.eval();
}

int DeclStat::execute()
{
  for (auto decl: init_decl_list)
    decl.eval();

  return 0;
}

int CompoundStatement::execute()
{
  for (auto stat: stat_list)
    stat.execute();
   
  return 0;
}


int Parser::parse(vector <Token>& tokens, vector<Statement>& stats)
{
  return 0;
}
