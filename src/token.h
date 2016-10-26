#ifndef TOKEN_H
#define TOKEN_H

#include <regex.h>

enum token_type {
    SP,         // ' ' | '\t'
    CRLF,       // \r\n
    COMMENT,    // /* asdf */ | // asdf
    INT_CONST,  // 123
    STRING,     // "hello"
    DECL,        // int
    IF,         // if
    ELSE,       // else
    WHILE,      // while
    DO,         // do
    FOR,        // for
    BREAK,      // break
    COMMA,      // ,
    SEMI_COLON, // ;
    LBRACE,     // {
    RBRACE,     // }
    LPAREN,     // (
    RPAREN,     // )
    EQ,         // ==
    NE,         // !=
    LE,         // <=
    GE,         // >=
    LT,         // <
    GT,         // >
    ASSIGN,     // =
    INC,        // ++
    DEC,        // --
    ADD,        // +
    SUB,        // -
    MUL,        // *
    DIV,        // /
    ID,         // variable_a
};

struct token {
    enum token_type type;
    char *literal;
    size_t length;
    size_t line;
};

void token_regex_init();

struct token token_scan(char *code);

#endif
