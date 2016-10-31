#include "parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

/* below are macros used only in this file */

/*Statment parsers, parse the string(str_begin, str_end) for a statement
  and return an iterator pointing to the rest of the string.
  Origin is the position of the current leading token.
*/
#define STAT_PARSER(T, fn)					\
  string::const_iterator					\
  Parser::PARSER_NAME(fn)(string::const_iterator str_begin,	\
			  string::const_iterator str_end,	\
			  size_t origin,			\
			  T& stat)

/* Always use these two macros toghether 
   First make an origin, and then use POS to calculate the absolute pos.
*/
#define MK_ORIGIN(it) auto _str_origin = it
#define POS(it) ((it) - _str_origin + origin)

void
Parser::parse_init_decl(string::const_iterator str_begin,
			string::const_iterator str_end,
			size_t origin,
			InitDecl& decl) 
{
  smatch m;
  decl.id = tokens[origin].code;
  parse_expr(str_begin+2, str_end, origin + 2, decl.expr);
}


STAT_PARSER(DeclStat, decl)
{

  smatch m;
  bool end = false;
  size_t len;
  InitDecl decl_expr;

  MK_ORIGIN(str_begin);

  str_begin++;
  for (; !end && str_begin != str_end; str_begin += len) {
    if (regex_search(str_begin, str_end, m, regex("a=[^,;]+"),
		     regex_constants::match_continuous)){
      //need ot output
      stat.has_init = true;
      
      len = m[0].str().length();
      parse_init_decl(m[0].first, m[0].second,
		      POS(m[0].first), decl_expr);
      stat.decl_list.push_back(decl_expr);
    } else if('a' == *str_begin) {
      len = 1;
      decl_expr.id = tokens[POS(str_begin)].code;
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

string::const_iterator
Parser::parse_stat_list(string::const_iterator str_begin,
			string::const_iterator str_end,
			size_t origin,
			vector< shared_ptr<Statement> >& stats)
{
  shared_ptr<Statement> stat_ptr;

  MK_ORIGIN(str_begin);

  while ((str_begin != str_end) && (*str_begin != '}')) {
    str_begin = parse_stat(str_begin, str_end, POS(str_begin), stat_ptr);
    stats.push_back(stat_ptr);
  }

  return str_begin;
}

STAT_PARSER(CompoundStat, comp)
{
  smatch m;
  string::const_iterator comp_end;

  // skip {
  ++str_begin;
  str_begin = parse_stat_list(str_begin, str_end, origin + 1, stat.stat_list);

  return str_begin + 1;
}


STAT_PARSER(PrintStat, print)
{
  smatch m;

  MK_ORIGIN(str_begin);
  
  str_begin+=2;
  regex_search(str_begin, str_end, m, regex("(.+?)\\);"),
	       regex_constants::match_continuous);

  parse_expr(m[1].first, m[1].second, POS(m[1].first), stat.expr);
  return m[0].second;
}

STAT_PARSER(SelectStat, select)
{
  smatch m;

  MK_ORIGIN(str_begin);
  
  //skip if(
  str_begin+=2;
  regex_search(str_begin, str_end, m, regex("(.+?)\\)"),
	       regex_constants::match_continuous);
  parse_expr(m[1].first, m[1].second, POS(m[1].first), stat.expr);

  // parse comp
  str_begin = m[0].second;
  str_begin = parse_stat(str_begin, str_end, POS(str_begin), stat.stat1);

  // else {}
  if (str_begin != str_end && *str_begin == '@')  {
    stat.has_else = true;
    str_begin++;
    str_begin = parse_stat(str_begin, str_end, POS(str_begin), stat.stat2);
  }

  return str_begin;
}

STAT_PARSER(ForStat, for)
{
  smatch m;

  MK_ORIGIN(str_begin);
  
  str_begin += 2;
  //is there any DeclStat?
  if (*str_begin == 'i') {
    stat.has_decl = true;
    str_begin = parse_decl_stat(str_begin, str_end, POS(str_begin), stat.decl);

  } else {
    regex_search(str_begin, str_end, m, regex("(.*?)[;\\)]"),
		 regex_constants::match_continuous);
    parse_expr(m[1].first, m[1].second, POS(m[1].first), stat.expr[0]);
    str_begin = m[0].second;
  }
  
  for (int i = 1; i < 3; ++i, str_begin = m[0].second) {
    regex_search(str_begin, str_end, m, regex("(.*?)[;\\)]"),
		 regex_constants::match_continuous);
    parse_expr(m[1].first, m[1].second, POS(m[1].first), stat.expr[i]);
  }

  return parse_stat(str_begin, str_end, POS(str_begin), stat.stat);
}

STAT_PARSER(WhileStat, while)
{
  smatch m;

  MK_ORIGIN(str_begin);
  
  str_begin += 2;
  regex_search(str_begin, str_end, m, regex("(.+?)\\)"),
	       regex_constants::match_continuous);
  parse_expr(m[1].first, m[1].second, POS(m[1].first), stat.expr);

  str_begin = m[0].second;
  return parse_stat(str_begin, str_end, POS(str_begin), stat.stat);
}

STAT_PARSER(BreakStat, break) { return str_begin + 2; }

STAT_PARSER(DoStat, do)
{
  smatch m;

  MK_ORIGIN(str_begin);
  
  ++str_begin;
  str_begin = parse_stat(str_begin, str_end, POS(str_begin), stat.stat);

  str_begin += 2;
  regex_search(str_begin, str_end, m, regex("(.+?)\\);"),
	       regex_constants::match_continuous);
  parse_expr(m[1].first, m[1].second, POS(m[1].first), stat.expr);
  stat.linum = tokens[POS(m[1].first)].linum;

  return m[0].second;  
}

STAT_PARSER(ExprStat, expr)
{
  smatch m;

  MK_ORIGIN(str_begin);

  if(';' == *str_begin)
    stat.is_empty = true;
  regex_search(str_begin, str_end, m, regex("(.*?);"),
	       regex_constants::match_continuous);
  parse_expr(m[1].first, m[1].second, POS(m[1].first), stat.expr);
    
  return m[0].second;
}

#undef STAT_PARSR


string::const_iterator
Parser::parse_stat(string::const_iterator str_begin,
		   string::const_iterator str_end,
		   size_t origin,
		   shared_ptr<Statement>& stat_ptr)
{
  if (str_begin == str_end)
    return str_begin;
  
  string::const_iterator stat_end;

#define PARSE_STAT(T, v)	{					\
    shared_ptr<T> v##_ptr = make_shared<T>(tokens[origin].linum);	\
    stat_end = PARSER_NAME(v)(str_begin, str_end, origin, *v##_ptr);	\
    stat_ptr = dynamic_pointer_cast<Statement>(v##_ptr);  }

  switch (*str_begin) {
  case 'i': PARSE_STAT(DeclStat, decl);
    break;
  case 'p': PARSE_STAT(PrintStat, print);
    break;
  case '{': PARSE_STAT(CompoundStat, comp);
    break;
  case '?': PARSE_STAT(SelectStat, select);
    break;
  case 'f': PARSE_STAT(ForStat, for);
    break;
  case 'w': PARSE_STAT(WhileStat, while);
    break;
  case 'd': PARSE_STAT(DoStat, do);
    break;
  case 'b': PARSE_STAT(BreakStat, break);
    break;
  default : PARSE_STAT(ExprStat, expr);
    break;
  }

  return stat_end;
}


#undef MK_ORIGIN
#undef POS
