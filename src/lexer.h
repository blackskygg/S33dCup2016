#ifndef LEXER_H
#define LEXER_H

#include <regex.h>

enum token_type {
    SP,         // ' ' | '\t'
    CRLF,       // \r\n
    COMMENT,    // /* asdf */ | // asdf
    INT_CONST,  // 123
    STRING,     // "hello"
    DECL,       // int
    IF,         // if
    ELSE,       // else
    WHILE,      // while
    DO,         // do
    FOR,        // for
    BREAK,      // break
    PRINTF,     // printf
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
    END,
};

struct token {
    enum token_type type;
    char *			literal;
    size_t			length;
    size_t			line;
};

extern struct token tokens[]; // 仅声明，定义在lexer.c中

void token_regex_init(); // 用来初始化正则数组的，静态储存因此不需要最后释放

struct token token_scan(char *code); // 从code中读一个token

size_t token_fill(char *code); // 填充所有token到tokens[]里

#endif
