#include "token.h"
#include <stdlib.h>

#ifndef TOKEN_TYPE_NUM
#define TOKEN_TYPE_NUM 33
#endif

const char *token_patterns[] = {
    "[ |\t]+",
    "\r\n",
    "(/\\*([^\\*]|(\\*[^/]))*\\*/)|(//([^\r]|(\r[^\n]))*)",
    "[1-9][0-9]*",
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
    struct token token = { 0, NULL, 0, 0 };

    for (size_t i = 0; i < TOKEN_TYPE_NUM; i++) {
        regmatch_t matched;
        int match_status = regexec(token_re + i, code, 1, &matched, 0);
        if (matched.rm_so == 0 && match_status != REG_NOMATCH) {
            token.type = (enum token_type)i;
            token.literal = code;
            token.length = (size_t)matched.rm_eo;
            token.line = line;
            for (size_t i = 0; i < token.length; i++)
                if (token.literal[i] == '\r')
                    line++;
            break;
        }
    }

    return token;
}
