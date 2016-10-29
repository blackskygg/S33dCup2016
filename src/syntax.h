#ifndef SYNTAX_H
#define SYNTAX_H

#include <regex.h>
#include "token.h"

enum syntax_type {
    SYN_STAT_LIST,
    SYN_DECL,
    SYN_INIT_DECL,
    SYN_EXP_STAT,
    SYN_COMPOUND_STAT,

    SYN_COND,
    SYN_BODY,
    SYN_ELSE,
    SYN_BOOTSTRAP,
    SYN_STEPIN,

    SYN_SELECTION_STAT,
    SYN_WHILE_STAT,
    SYN_DO_WHILE_STAT,
    SYN_FOR_STAT,

    SYN_JUMP_STAT,
    SYN_PRINT_STAT,
    SYN_ARGUMENT_LIST,
    SYN_ASSIGNMENT_EXP,
    SYN_EQUALITY_EXP,
    SYN_RELATIONAL_EXP,
    SYN_ADDITIVE_EXP,
    SYN_MULT_EXP,
    SYN_UNARY_EXP,
    SYN_POSTFIX_EXP,
    SYN_ID,
    SYN_INT_CONST,
    SYN_STRING,
    SYN_POS,
    SYN_NEG
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
struct syntax_node *expression(size_t *idx);
struct syntax_node *assignment_exp(size_t *idx);
struct syntax_node *equality_exp(size_t *idx);
struct syntax_node *relational_exp(size_t *idx);
struct syntax_node *additive_exp(size_t *idx);
struct syntax_node *mult_exp(size_t *idx);
struct syntax_node *unary_exp(size_t *idx);
struct syntax_node *postfix_exp(size_t *idx);
struct syntax_node *primary_exp(size_t *idx);

// fix l-comb exps' ast
struct syntax_node *rotate_exp(struct syntax_node *root);

#endif
