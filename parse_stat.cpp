#include "parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void Parser::parse_init_decl(string::const_iterator str_begin,
			     string::const_iterator str_end,
			     size_t origin,
			     InitDecl& decl) 
{
  smatch m;
  decl.id = tokens[origin].code;
  parse_expr(str_begin+2, str_end, origin + 2, decl.expr);
}

string::const_iterator Parser::parse_decl_stat(string::const_iterator str_begin,
					       string::const_iterator str_end,
					       size_t origin,
					       DeclStat& stat)
{

  smatch m;
  bool end;
  size_t len, begin_pos;
  auto str_origin = str_begin;
  InitDecl decl_expr;

  str_begin++;
  for (; !end && str_begin != str_end; str_begin += len) {
    begin_pos = str_begin - str_origin + origin;
    if (regex_search(str_begin, str_end, m, regex("a=[^,;]+"),
		     regex_constants::match_continuous)){
      //need ot output
      stat.has_init = true;
      
      len = m[0].str().length();
      parse_init_decl(m[0].first, m[0].second,
		      begin_pos, decl_expr);
      stat.decl_list.push_back(decl_expr);

    } else if('a' == *str_begin) {
      len = 1;
      decl_expr.id = tokens[begin_pos].code;
      decl_expr.expr = dynamic_pointer_cast<Expression>	
	(make_shared<PrimaryExprConst>(0));
      stat.decl_list.push_back(decl_expr);
      
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

  regex_search(str_begin, str_end, m, regex("\\{(.*?)\\}"),
	       regex_constants::match_continuous);
  parse_stat_list(m[1].first, m[1].second, origin + 1,
		  stat.stat_list, stat.scope);
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
  regex_search(str_begin, str_end, m, regex("(.+?)\\);"),
	       regex_constants::match_continuous);
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
  regex_search(str_begin, str_end, m, regex("(.+?)\\)"),
	       regex_constants::match_continuous);
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
  int i = 0;

  str_begin += 2;

  //is there any DeclStat?
  if (*str_begin == 'i') {
    stat.has_decl = true;
    pos = str_begin - str_origin + origin;
    str_begin = parse_decl_stat(str_begin, str_end, pos, stat.decl);
  } else {
    regex_search(str_begin, str_end, m, regex("(.*?)[;\\)]"),
		 regex_constants::match_continuous);
    pos = m[1].first - str_origin + origin;
    parse_expr(m[1].first, m[1].second, pos, stat.expr[0]);
    str_begin = m[0].second;
  }
  
  for (int i = 1; i < 3; ++i, str_begin = m[0].second) {
    regex_search(str_begin, str_end, m, regex("(.*?)[;\\)]"),
		 regex_constants::match_continuous);
    pos = m[1].first - str_origin + origin;
    parse_expr(m[1].first, m[1].second, pos, stat.expr[i]);
  }

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

string::const_iterator Parser::parse_break_stat(string::const_iterator str_begin,
						string::const_iterator str_end,
						size_t origin,
						BreakStat& stat)
{
  return str_begin + 2;
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
  if (';' == *str_begin)
    return str_begin + 1;
  
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

#define PARSE_STAT(T, v, scp)	{					\
    shared_ptr<T> v##_ptr = make_shared<T>(scp, tokens[origin].linum);	\
    stat_end = parse_##v##_stat(str_begin, str_end, origin, *v##_ptr);	\
    stat_ptr = dynamic_pointer_cast<Statement>(v##_ptr);  }

  cout << string(str_begin, str_end) <<endl;
  cout << *str_begin << " linum: "<< tokens[origin].linum <<endl;
  switch (*str_begin) {
  case 'i': PARSE_STAT(DeclStat, decl, scope);
    break;
  case 'p': PARSE_STAT(PrintStat, print, scope);
    break;
  case '{': PARSE_STAT(CompoundStat, comp, make_shared<Scope>(scope));
    break;
  case '?': PARSE_STAT(SelectStat, select, scope);
    break;
  case 'f': PARSE_STAT(ForStat, for, make_shared<Scope>(scope));
    break;
  case 'w': PARSE_STAT(WhileStat, while, scope);
    break;
  case 'd': PARSE_STAT(DoStat, do, scope);
    break;
  case 'b': PARSE_STAT(BreakStat, break, scope);
    break;
  default: PARSE_STAT(ExprStat, expr, scope);
    break;
  }

  return stat_end;
  
#undef PARSE_STAT
}
