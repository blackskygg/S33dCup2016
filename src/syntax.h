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
    WHILE_STAT,
    DO_WHILE_STAT,
    FOR_STAT,
    JUMP_STAT,
    PRINT_STAT,
    ARGUMENT_LIST,
    EXPRESSION,
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
    enum syntax_type	type;
    size_t				token_idx;
    struct syntax_node *children;
    struct syntax_node *sibling;
};

struct syntax_node *generate_ast();
void destory_ast(struct syntax_node *root);
void print_ast(struct syntax_node *root, size_t level);

struct syntax_node *stat_list(size_t *idx);
struct syntax_node *stat(size_t *idx);
struct syntax_node *decl_stat(size_t *idx);
struct syntax_node *init_decl_list(size_t *idx);
struct syntax_node *init_decl(size_t *idx);
struct syntax_node *exp_stat(size_t *idx);
struct syntax_node *compound_stat(size_t *idx);
struct syntax_node *selection_stat(size_t *idx);
struct syntax_node *while_stat(size_t *idx);
struct syntax_node *do_while_stat(size_t *idx);
struct syntax_node *for_stat(size_t *idx);
struct syntax_node *jump_stat(size_t *idx);
struct syntax_node *print_stat(size_t *idx);
struct syntax_node *argument_list(size_t *idx);
struct syntax_node *expression(size_t *idx);


#endif
