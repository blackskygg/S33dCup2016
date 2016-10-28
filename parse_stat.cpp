#include "parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

string::const_iterator Parser::parse_decl_stat(string::const_iterator str_begin,
					       string::const_iterator str_end,
					       size_t origin,
					       DeclStat& stat)
{

  smatch m;
  bool end;
  size_t len, begin_pos;
  auto str_origin = str_begin;
  AssignmentExpr assign_expr;

  str_begin++;
  for (; !end && str_begin != str_end; str_begin += len) {
    begin_pos = str_begin - str_origin + origin;
    if (regex_search(str_begin, str_end, m, regex("a=[^,;]"),
		      regex_constants::match_continuous)){
      len = m[0].str().length();
      parse_assign_expr(m[0].first, m[0].second,
			  begin_pos, assign_expr);
      stat.decl_list.push_back(assign_expr);
    } else if('a' == *str_begin) {
      len = 1;
      assign_expr.id = tokens[begin_pos].code;
      assign_expr.expr = dynamic_pointer_cast<Expression>	
	(make_shared<PrimaryExprConst>(0));
      stat.decl_list.push_back(assign_expr);
    } else if(',' == *str_begin) {
      len = 1;
    } else if(';' == *str_begin) {
      len = 1;
      end = true;
    }
  }
    
  for(auto e: stat.decl_list) {
    cout<<(dynamic_pointer_cast<PrimaryExprConst>(e.expr))->val<<endl;
  }

  return str_begin;
}

void Parser::parse_stat_list(string::const_iterator str_begin,
			 string::const_iterator str_end,
			 size_t origin,
			 vector< shared_ptr<Statement> >& stats,
			 shared_ptr<Scope> scope)
{
  size_t  begin_pos;
  auto str_origin = str_begin;
  shared_ptr<Statement> stat_ptr;

  while (str_begin != str_end) {
    begin_pos = str_begin - str_origin + origin;
    str_begin = parse_stat(str_begin, str_end, begin_pos, stat_ptr, scope);
    stats.push_back(stat_ptr);
  }
}


string::const_iterator Parser::parse_comp_stat(string::const_iterator str_begin,
					       string::const_iterator str_end,
					       size_t origin,
					       CompoundStat& stat)
{
  smatch m;
  string::const_iterator comp_end;

  regex_search(str_begin, str_end, m, regex("{.*?}"));
  parse_stat_list(m[0].first, m[0].second, origin, stat.stat_list, make_shared<Scope>());
  return m[0].second;
}

string::const_iterator Parser::parse_select_stat(string::const_iterator str_begin,
					       string::const_iterator str_end,
					       size_t origin,
					       SelectStat& stat)
{
}
			

string::const_iterator Parser::parse_stat(string::const_iterator str_begin,
					  string::const_iterator str_end,
					  size_t orgin,
					  shared_ptr<Statement>& stat_ptr,
					  std::shared_ptr<Scope> scope)
{
  Statement::StatType type;
  auto str_origin = str_begin;
  string::const_iterator stat_end;
  
  shared_ptr<DeclStat> decl_ptr;
  shared_ptr<CompoundStat> comp_ptr;
  shared_ptr<SelectStat> select_ptr;

  switch (*str_begin) {
  case 'i':
    decl_ptr = make_shared<DeclStat>(scope);
    stat_end = parse_decl_stat(str_begin, str_end, 0, *decl_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(decl_ptr);
    break;
  case '{':
    comp_ptr = make_shared<CompoundStat>(scope);
    stat_end = parse_comp_stat(str_begin, str_end, 0, *comp_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(comp_ptr);
    break;
  case '?':
    select_ptr = make_shared<SelectStat>(scope);
    stat_end = parse_select_stat(str_begin, str_end, 0, *select_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(select_ptr);
    break;
  case 'f':
    break;
  case 'w':
    break;
  case 'd':
    break;
  default:
    break;
  }

  return stat_end;
}
