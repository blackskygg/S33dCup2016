#ifndef SYNTAX_H
#define SYNTAX_H

#include <regex.h>
#include "token.h"

enum syntax_type {
    STAT_LIST,
    SYN_DECL,
    INIT_DECL_LIST,
    INIT_DECL,
    STAT,
    EXP_STAT,
    COMPOUND_STAT,
    SELECTION_STAT,
    ITERATION_STAT,
    JUMP_STAT,
    PRINT_STAT,
    ARGUMENT_LIST,
    EXP,
    ASSIGNMENT_EXP,
    EQUALITY_EXP,
    RELATIONAL_EXP,
    ADDITIVE_EXP,
    MULT_EXP,
    UNARY_EXP,
    POSTFIX_EXP,
    PRIMARY_EXP,
    SYN_INT_CONST,
    SYN_ID,
    SYN_STRING,
};

struct syntax_node {
    enum syntax_type type;
    union {
        int		int_const;
        char *	id;
        char *	string;
    } info;
};

#endif
