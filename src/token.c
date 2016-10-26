#include "token.h"
#include <stdio.h>
#include <string.h>

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

regex_t token_re[32];

void token_regex_init()
{
    for (size_t i = 0; i < 32; i++) {
        int status = regcomp(token_re + i, token_patterns[i], REG_EXTENDED);
        if (status)
            printf("%ld failed \"%s\"\n", i, token_patterns[i]);
    }
    return;
}

struct token token_scan(char *code)
{
    static int line_num = 1;
    struct token token = { 0, NULL, 0, 0 };

    for (size_t i = 0; i < 32; i++) {
        regmatch_t matched;
        int match_status = regexec(token_re + i, code, 1, &matched, 0);
        if (matched.rm_so == 0 && match_status != REG_NOMATCH) {
            token.type = (enum token_type)i;
            token.token = code;
            token.token_len = (size_t)matched.rm_eo;
            token.line_num = line_num;
            for (size_t i = 0; i < token.token_len; i++)
                if (token.token[i] == '\r')
                    line_num++;
            break;
        }
    }

    return token;
}
