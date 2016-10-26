#ifndef TOKEN_H
#define TOKEN_H

#include <regex.h>

enum token_type {
    INT_CONST,
    ID,
    STRING,
    INT,
    SEMI_COLON,
};

struct token_regex {
    regex_t re;
};

#endif
