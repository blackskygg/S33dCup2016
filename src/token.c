#include "token.h"
#include <stdlib.h>

#ifndef TOKEN_TYPE_NUM
#define TOKEN_TYPE_NUM 33
#endif

const char *token_patterns[] = {
    "[ |\t]+",
    "\r\n",
    "(/\\*([^\\*]|(\\*[^/]))*\\*/)|(//([^\r]|(\r[^\n]))*)",
    "0|[1-9][0-9]*",
    "\"(\\.|[^\"])*\"",
    "int",
    "if",
    "else",
    "while",
    "do",
    "for",
    "break",
    "printf",
    ",",
    ";",
    "\\{",
    "\\}",
    "\\(",
    "\\)",
    "==",
    "!=",
    "<=",
    ">=",
    "<",
    ">",
    "=",
    "\\+\\+",
    "--",
    "\\+",
    "-",
    "\\*",
    "/",
    "[_a-zA-Z][_a-zA-Z0-9]*",
};

regex_t token_re[TOKEN_TYPE_NUM];

void token_regex_init()
{
    for (size_t i = 0; i < TOKEN_TYPE_NUM; i++)
        regcomp(token_re + i, token_patterns[i], REG_EXTENDED);
    return;
}

struct token token_scan(char *code)
{
    static int line = 1;
    struct token token = { SP, NULL, 0, 0 };

    size_t max_match_len = 0;
    enum token_type max_match_type = (enum token_type)0;

    regmatch_t matched;

    for (size_t i = 0; i < TOKEN_TYPE_NUM; i++) {
        int match_status = regexec(token_re + i, code, 1, &matched, 0);
        if (matched.rm_so == 0 && match_status != REG_NOMATCH) {
            if (max_match_len < matched.rm_eo) {
                max_match_len = matched.rm_eo;
                max_match_type = (enum token_type)i;
            }
        }
    }

    regexec(token_re + (size_t)max_match_type, code, 1, &matched, 0);
    token.type = max_match_type;
    token.literal = code;
    token.length = (size_t)matched.rm_eo;
    token.line = line;
    for (size_t i = 0; i < token.length; i++)
        if (token.literal[i] == '\r')
            line++;

    return token;
}
