#include "parser.h"

using namespace std;

long long Scope::get_identifier(const string& name)
{
  unordered_map<string, long long>::iterator it;

  if ((vars.end() == (it = vars.find(name))) && (parent != NULL))
    return parent->get_identifier(name);
  else
    return it->second;
}

void Scope::mod_identifier(const string& name, long long val)
{
  unordered_map<string, long long>::iterator it;

  if ((vars.end() == (it = vars.find(name))) && (parent != NULL))
    parent->mod_identifier(name, val);
  else
    vars[name] = val;
}

void Scope::add_identifier(const string& name, long long val)
{
  vars[name] = val;
}

void Result::add_line(int linum) {
  if (linum != prev) {
    prev = linum;
    lines.push_back(linum);
  }
}

void Result::print(ofstream& ofs)
{
  if (0 == lines.size()) return;

  int n = lines.back();
  lines.pop_back();
  for(auto ln: lines)
    ofs << ln << " ";
  ofs << n;
}


Parser::Parser(vector <Token> &tokens) : tokens(tokens)
{
  /* we use this code map to encode the tokens list into a string
   * so that we can use regular expression
   * to simply the state machine construction
   */
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
  code_map[Token::STRING_LITERAL] = 's';

  /* since we've encoded the token list,
   * we can now use normal regex to handle them
   */
#define EXPR_TPL(T, R) expr_tpls.push_back(make_pair(Expression::T, regex(R)))
  EXPR_TPL(COMMA, "(.+),(.+)");
  EXPR_TPL(ASSIGNMENT, "(.+?)=(.+)");
  EXPR_TPL(EQUALITY, "(.+)([!e])(.+)");
  EXPR_TPL(RELATIONAL, "(.+)([gl><])(.+)");
  EXPR_TPL(ADDITIVE, "(.*[\\^vnas])([\\+-])(.+)");
  EXPR_TPL(MULT, "(.+)([\\*/])(.+)");
  EXPR_TPL(UNARY, "([\\+-])(.+)");
  EXPR_TPL(POSTFIX, "(.+)([\\^v])");
  EXPR_TPL(PRIMARY, "([nas])");
#undef EXPR_TPL
}

void Parser::encode_tokens(std::vector <Token>& tokens, string &s)
{
  for (auto token: tokens)
    s.push_back(code_map[token.type]);
}


void Parser::parse(Result& result)
{
  string s;
  Scope global;
  CompoundStat stat(1); //we see the whole program as a big CompoundStat

  encode_tokens(tokens, s);
  parse_stat_list(s.cbegin(), s.cend(), 0, stat.stat_list);
  stat.execute(result, global);
}
