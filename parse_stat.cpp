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
    
  //  for(auto e: stat.decl_list) {
  //    cout<<(dynamic_pointer_cast<PrimaryExprConst>(e.expr))->val<<endl;
  //  }

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

  regex_search(str_begin, str_end, m, regex("\\{(.*?)\\}"), regex_constants::match_continuous);
  parse_stat_list(m[1].first, m[1].second, origin + 1, stat.stat_list, make_shared<Scope>());
  return m[0].second;
}


string::const_iterator Parser::parse_print_stat(string::const_iterator str_begin,
					       string::const_iterator str_end,
					       size_t origin,
					       PrintStat& stat)
{
  auto str_origin = str_begin;
  smatch m;
  size_t pos;

  str_begin+=2;
  regex_search(str_begin, str_end, m, regex("(.+?)\\);"), regex_constants::match_continuous);
  pos = m[1].first - str_origin + origin;
  parse_expr(m[1].first, m[1].second, pos, stat.expr);

  str_begin = m[0].second;
  return str_begin;
}

string::const_iterator Parser::parse_select_stat(string::const_iterator str_begin,
					       string::const_iterator str_end,
					       size_t origin,
					       SelectStat& stat)
{
  auto str_origin = str_begin;
  smatch m;
  size_t pos;

  //if(expr)
  str_begin+=2;
  regex_search(str_begin, str_end, m, regex("(.+?)\\)"), regex_constants::match_continuous);
  pos = m[1].first - str_origin + origin;
  parse_expr(m[1].first, m[1].second, pos, stat.expr);

  // {}
  str_begin = m[0].second;
  pos = str_begin - str_origin + origin;
  str_begin = parse_stat(str_begin, str_end, pos, stat.stat1, stat.scope);

  // else {}
  if (str_begin != str_end && *str_begin == '@')  {
    str_begin++;
    pos = str_begin - str_origin + origin;
    str_begin = parse_stat(str_begin, str_end, pos, stat.stat2, stat.scope);
  }

  return str_begin;
}

string::const_iterator Parser::parse_for_stat(string::const_iterator str_begin,
					      string::const_iterator str_end,
					      size_t origin,
					      ForStat& stat)
{
  auto str_origin = str_begin;
  smatch m;
  size_t pos;

  str_begin += 2;
  for (int i = 0; i < 3; ++i, str_begin = m[0].second) {
    //we use a non-capturing group to capture the potential 'int'
    //, and treat the rest as expr
    regex_search(str_begin, str_end, m, regex("(?:i)?(.*?)[;\\)]"), regex_constants::match_continuous);
    pos = m[1].first - str_origin + origin;
    parse_expr(m[1].first, m[1].second, pos, stat.expr[i]);
  }

  pos = str_begin - str_origin + origin;
  str_begin = parse_stat(str_begin, str_end, pos, stat.stat, stat.scope);
  
  return str_begin;
}

string::const_iterator Parser::parse_while_stat(string::const_iterator str_begin,
					      string::const_iterator str_end,
					      size_t origin,
					      WhileStat& stat)
{
  auto str_origin = str_begin;
  smatch m;
  size_t pos;


  str_begin += 2;
  regex_search(str_begin, str_end, m, regex("(.+?)\\)"), regex_constants::match_continuous);
  pos = m[1].first - str_origin + origin;
  parse_expr(m[1].first, m[1].second, pos, stat.expr);

  str_begin = m[0].second;
  pos = str_begin - str_origin + origin;
  str_begin = parse_stat(str_begin, str_end, pos, stat.stat, stat.scope);

  return str_begin;
}

string::const_iterator Parser::parse_do_stat(string::const_iterator str_begin,
					     string::const_iterator str_end,
					     size_t origin,
					     DoStat& stat)
{
  auto str_origin = str_begin;
  smatch m;
  size_t pos;

  ++str_begin;
  pos = str_begin - str_origin + origin;
  str_begin = parse_stat(str_begin, str_end, pos, stat.stat, stat.scope);

  str_begin += 2;
  regex_search(str_begin, str_end, m, regex("(.+?)\\);"), regex_constants::match_continuous);
  pos = m[1].first - str_origin + origin;
  parse_expr(m[1].first, m[1].second, pos, stat.expr);
  str_begin = m[0].second;  

  return str_begin;
}

string::const_iterator Parser::parse_expr_stat(string::const_iterator str_begin,
					       string::const_iterator str_end,
					       size_t origin,
					       ExprStat& stat) 
{
  auto str_origin = str_begin;
  smatch m;
  size_t pos;

  //ExprStat could be empty
  regex_search(str_begin, str_end, m, regex("(.*?);"), regex_constants::match_continuous);
  pos = m[1].first - str_origin + origin;
  parse_expr(m[1].first, m[1].second, pos, stat.expr);
  str_begin = m[0].second;
    
  return str_begin;
}

string::const_iterator Parser::parse_stat(string::const_iterator str_begin,
					  string::const_iterator str_end,
					  size_t origin,
					  shared_ptr<Statement>& stat_ptr,
					  std::shared_ptr<Scope> scope)
{
  if (str_begin == str_end)
    return str_begin;
  
  auto str_origin = str_begin;
  string::const_iterator stat_end;
  
  shared_ptr<DeclStat> decl_ptr;
  shared_ptr<CompoundStat> comp_ptr;
  shared_ptr<SelectStat> select_ptr;
  shared_ptr<ForStat> for_ptr;
  shared_ptr<WhileStat> while_ptr;
  shared_ptr<DoStat> do_ptr;
  shared_ptr<PrintStat> print_ptr;
  shared_ptr<ExprStat> expr_ptr;

  cout << *str_begin << " linum: "<< tokens[origin].linum <<endl;
  switch (*str_begin) {
  case 'i':
    decl_ptr = make_shared<DeclStat>(scope, tokens[origin].linum);
    stat_end = parse_decl_stat(str_begin, str_end, origin, *decl_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(decl_ptr);
    break;
  case 'p':
    print_ptr = make_shared<PrintStat>(scope, tokens[origin].linum);
    stat_end = parse_print_stat(str_begin, str_end, origin, *print_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(print_ptr);
    break;
  case '{':
    comp_ptr = make_shared<CompoundStat>(scope, tokens[origin].linum);
    stat_end = parse_comp_stat(str_begin, str_end, origin, *comp_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(comp_ptr);
    break;
  case '?':
    select_ptr = make_shared<SelectStat>(scope, tokens[origin].linum);
    stat_end = parse_select_stat(str_begin, str_end, origin, *select_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(select_ptr);
    break;
  case 'f':
    //for has its own scope
    for_ptr = make_shared<ForStat>(make_shared<Scope>(), tokens[origin].linum);
    stat_end = parse_for_stat(str_begin, str_end, origin, *for_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(for_ptr);
    break;
  case 'w':
    while_ptr = make_shared<WhileStat>(scope, tokens[origin].linum);
    stat_end = parse_while_stat(str_begin, str_end, origin, *while_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(while_ptr);
    break;
  case 'd':
    do_ptr = make_shared<DoStat>(scope, tokens[origin].linum);
    stat_end = parse_do_stat(str_begin, str_end, origin, *do_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(do_ptr);
    break;
  default:
    expr_ptr = make_shared<ExprStat>(scope, tokens[origin].linum);
    stat_end = parse_expr_stat(str_begin, str_end, origin, *expr_ptr);
    stat_ptr = dynamic_pointer_cast<Statement>(expr_ptr);
    break;
  }

  return stat_end;
}
