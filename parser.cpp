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
  return scope.get_identifier(id).val = expr.eval(scope);
}

int DeclStat::execute()
{
  return 0;
}

int CompoundStatement::execute()
{
  for (auto stat: stat_list)
    stat.execute();
   
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

  stat_tpls.push_back(make_pair(Statement::COMPOUND_STAT, stat_regex("\\{(.*?)\\}")));
  stat_tpls.push_back(make_pair(Statement::DECL_STAT, stat_regex("i(.+?);")));

  expr_tpls.push_back(make_pair(Expression::COMMA, expr_regex("(.+?),(.+)")));
  expr_tpls.push_back(make_pair(Expression::ASSIGNMENT, expr_regex("(.+?)=(.+)")));
  expr_tpls.push_back(make_pair(Expression::EQUALITY, expr_regex("(.+?)[!e](.+)")));
  expr_tpls.push_back(make_pair(Expression::RELATIONAL, expr_regex("(.+?)[gl><](.+)")));
  expr_tpls.push_back(make_pair(Expression::ADDITIVE, expr_regex("(.+?)[\\+-](.+)")));
  expr_tpls.push_back(make_pair(Expression::MULT, expr_regex("(.+?)[\\*/](.+)")));
  expr_tpls.push_back(make_pair(Expression::UNARY, expr_regex("[\\+-](.+)")));
  expr_tpls.push_back(make_pair(Expression::POSTFIX, expr_regex(".+")));
  expr_tpls.push_back(make_pair(Expression::PRIMARY, expr_regex("[na]")));
}

void Parser::parse_expr(string::const_iterator str_begin,
			string::const_iterator str_end,
			size_t origin,
			Expression& expr)
{
  smatch m;
  Expression::ExprType type;
  size_t begin_pos, end_pos;
  auto str_origin = str_begin;

  for ( ; str_begin != str_end; str_begin += m[0].length()) {
    //search for best match
    for (auto tpl: stat_tpls) {
      if (regex_search(str_begin, str_end, m, tpl.second,
		       regex_constants::match_continuous))
	break;
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
      if (m[0].str() == "n") 
	expr = PrimaryExprConst(atoi(tokens[begin_pos].code.c_str()));
      else
        expr = PrimaryExprId(tokens[begin_pos].code.c_str());
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


void Parser::parse_decl_stat(string::const_iterator str_begin,
			     string::const_iterator str_end,
			     size_t origin,
			     DeclStat& stat)
{
  smatch m;
  AssignmentExpr assign_expr;
  
  regex_search(str_begin, str_end, m, regex("a|a=[^,;]+"));
  for (size_t i = 0; i < m.size(); ++i) {
    if (m.str().length() > 1) {
      parse_assign_expr(m[i].first, m[i].second,
			origin + m.position(i), assign_expr);
    } else {
      assign_expr.id = tokens[origin + m.position(i)].code;
      assign_expr.expr = PrimaryExprConst(0);
    }
    stat.decl_list.push_back(assign_expr);

  }
}

void Parser::parse_stats(string::const_iterator str_begin,
			 string::const_iterator str_end,
			 size_t origin,
			 vector<Statement>& stats,
			 vector<Token> &tokens, Scope *scope)
{
  size_t max_len, begin_pos, end_pos;
  smatch m, best_match;
  Statement::StatType type;
  auto str_origin = str_begin;
  DeclStat decl_stat(scope);

  for ( ; str_begin != str_end; str_begin += max_len) {
    max_len = 0;
    //search for best match
    for (auto tpl: stat_tpls) {
      if (regex_search(str_begin, str_end, m, tpl.second,
		       regex_constants::match_continuous)
	  && m[0].length() > max_len) {
	best_match = move(m);
	max_len = best_match[0].length();
	type = tpl.first;
      }
    }

    if (!max_len)
      break;

    begin_pos = str_begin - str_origin + origin;
    end_pos = begin_pos + max_len;

    cout << "stat_type: " << type << endl;
    switch (type) {
    case Statement::COMPOUND_STAT:
      break;

    case Statement::DECL_STAT:
      decl_stat.begin = begin_pos;
      decl_stat.end = end_pos;
      parse_decl_stat(best_match[0].first, best_match[0].second,
		      begin_pos, decl_stat);
      stats.push_back(decl_stat);
      break;

    default:
      break;
    }
  }
}

void Parser::encode_tokens(std::vector <Token>& tokens, string &s)
{
  for (auto token: tokens) 
    s.push_back(code_map[token.type]);
}


int Parser::parse(vector<Statement>& stats)
{
  string s;
  Scope global_scp;

  encode_tokens(tokens, s);
  parse_stats(s.cbegin(), s.cend(), 0, stats, tokens, &global_scp);

  return 0;
}
