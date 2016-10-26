#ifndef TOKEN_H
#define TOKEN_H

#include <regex.h>

enum token_type {
    INT_CONST,  // 123
    STRING,     // "hello"
    INT,        // int
    IF,         // if
    ELSE,       // else
    WHILE,      // while
    DO,         // do
    FOR,        // for
    BREAK,      // break
    ID,         // variable_a
    COMMA,      // ,
    SEMI_COLON, // ;
    LBRACE,     // {
    RBRACE,     // }
    LPAREN,     // (
    RPAREN,     // )
    EQ,         // ==
    NE,         // !=
    LT,         // <
    GT,         // >
    LE,         // <=
    GE,         // >=
    ASSIGN,     // = | += | -= | *= | /=
    INC,        // ++
    DEC,        // --
    ADD,        // +
    SUB,        // -
    MUL,        // *
    DIV,        // /
};

struct token {
    enum token_type type;
    char *content;
};

#endif
