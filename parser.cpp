#include "parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Identifier& Scope::get_identifier(string name)
{
}

int PrimaryExprConst:: eval(Scope& scope)
{
  return val;
}

int PrimaryExprId:: eval(Scope& scope)
{
  return scope.get_identifier(id).val;
}

int AssignmentExpr::eval(Scope& scope)
{
  return scope.get_identifier(id).val = expr->eval(scope);
}

int DeclStat::execute()
{
  return 0;
}

int CompoundStat::execute()
{
  for (auto stat: stat_list)
    stat->execute();
   
  return 0;
}

Parser::Parser(vector <Token> &tokens): tokens(tokens)
{
  //  code_map[BLANK] = '';
  code_map[Token::INT_TYPE] = 'i';
  code_map[Token::FOR] = 'f';
  code_map[Token::IF] = '?';
  code_map[Token::ELSE] = '@';
  code_map[Token::DO] = 'd';
  code_map[Token::WHILE] = 'w';
  code_map[Token::BREAK] = 'b';
  code_map[Token::PRINTF] = 'p';
  code_map[Token::IDENTIFIER] = 'a';
  code_map[Token::NUMBER] = 'n';
  code_map[Token::RP] = ')';
  code_map[Token::LP] = '(';
  code_map[Token::LB] = '{';
  code_map[Token::RB] = '}';
  code_map[Token::ADD] = '+';
  code_map[Token::SUB] = '-';
  code_map[Token::MUL] = '*';
  code_map[Token::DIV] = '/';
  code_map[Token::DEC] = 'v';
  code_map[Token::INC] = '^';
  code_map[Token::ASSIGNMENT] = '=';
  code_map[Token::NE] = '!';
  code_map[Token::GE] = 'g';
  code_map[Token::LE] = 'l';
  code_map[Token::GT] = '>';
  code_map[Token::LT] = '<';
  code_map[Token::EQ] = 'e';
  code_map[Token::COLON] = ':';
  code_map[Token::COMMA] = ',';
  code_map[Token::SEMICOLON] = ';';
  //  code_map[Token::COMMENT] = '';
  //  code_map[Token::CRLF] = '';
  code_map[Token::STRING_LITERAL] = 's';


  stat_tpls.push_back(make_pair(Statement::DECL_STAT, stat_regex("i(.+?);")));
  stat_tpls.push_back(make_pair(Statement::COMPOUND_STAT, stat_regex("\\{(.*?)\\};?")));
  /*
  stat_tpls.push_back(make_pair(Statement::SELECTION_STAT_DO,
				stat_regex("\\?" "(?:\\(([^\\)]+)\\))"
					   "([^@;\\]+)?"
					   "(?:@([^;]+))?;")));
  stat_tpls.push_back(make_pair(Statement::ITERATIONSTAT_STAT_WHILE,
				stat_regex("w\\((.+?)\\)(.+?;)")));
  stat_tpls.push_back(make_pair(Statement::ITERATION_STAT, stat_regex("i(.+?);")));
  stat_tpls.push_back(make_pair(Statement::COMPOUND_STAT, stat_regex("\\{(.*?)\\}")));
  stat_tpls.push_back(make_pair(Statement::DECL_STAT, stat_regex("i(.+?);")));
  stat_tpls.push_back(make_pair(Statement::COMPOUND_STAT, stat_regex("\\{(.*?)\\}")));
  stat_tpls.push_back(make_pair(Statement::DECL_STAT, stat_regex("i(.+?);")));
  */
  expr_tpls.push_back(make_pair(Expression::COMMA, expr_regex("(.+?),(.+)")));
  expr_tpls.push_back(make_pair(Expression::ASSIGNMENT, expr_regex("(.+?)=(.+)")));
  expr_tpls.push_back(make_pair(Expression::EQUALITY, expr_regex("(.+?)[!e](.+)")));
  expr_tpls.push_back(make_pair(Expression::RELATIONAL, expr_regex("(.+?)[gl><](.+)")));
  expr_tpls.push_back(make_pair(Expression::ADDITIVE, expr_regex("(.+?)[\\+-](.+)")));
  expr_tpls.push_back(make_pair(Expression::MULT, expr_regex("(.+?)[\\*/](.+)")));
  expr_tpls.push_back(make_pair(Expression::UNARY, expr_regex("[\\+-](.+)")));
  expr_tpls.push_back(make_pair(Expression::POSTFIX, expr_regex("(.+?)[\\^v]")));
  expr_tpls.push_back(make_pair(Expression::PRIMARY, expr_regex("[na]")));
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

void Parser::parse_assign_expr(string::const_iterator str_begin,
			       string::const_iterator str_end,
			       size_t origin,
			       AssignmentExpr& expr) 
{
  smatch m;
  expr.id = tokens[origin].code;
  parse_expr(str_begin+2, str_end, origin + 2, expr.expr);
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

string::const_iterator Parser::parse_comp_stat(string::const_iterator str_begin,
			     string::const_iterator str_end,
			     size_t origin,
					       CompoundStat& stat)
{
  smatch m;
  string::const_iterator comp_end;

  regex_search(str_begin, str_end, m, regex("{.*?}"));
  parse_stats(m[0].first, m[0].second, origin, stat.stat_list, make_shared<Scope>());
  return m[0].second;
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
			
void Parser::parse_stats(string::const_iterator str_begin,
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

void Parser::encode_tokens(std::vector <Token>& tokens, string &s)
{
  for (auto token: tokens) 
    s.push_back(code_map[token.type]);
}


int Parser::parse(vector< shared_ptr<Statement> >& stats)
{
  string s;
  shared_ptr<Scope> global_scp;

  encode_tokens(tokens, s);
  parse_stats(s.cbegin(), s.cend(), 0, stats, global_scp);

  return 0;
}
