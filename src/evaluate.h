#ifndef EVALUATE_H
#define EVALUATE_H

#include <stdio.h>

#include "parser.h"

struct scope_record {
    char *					key;                    // NULL key represents scope sentinel
    size_t					len;
    int						val;
    struct scope_record *	prev;
};

extern struct scope_record *scope;
extern FILE *fout;

// 操作变量作用域，参见文档说明
void enter_scope();
void leave_scope();
int keycmp(char *k1, size_t l1, char *k2, size_t l2);
struct scope_record *new_record(char *key, size_t len, int val);
int get_record(char *key, size_t len);
void set_record(char *key, size_t len, int val);

// 对具体结构求值
int eval_stat_list(struct syntax_node *root);
int eval_stat(struct syntax_node *root);
int eval_decl_stat(struct syntax_node *root);
int eval_init_decl_list(struct syntax_node *root);
int eval_init_decl(struct syntax_node *root);
int eval_exp_stat(struct syntax_node *root);
int eval_compound_stat(struct syntax_node *root);
int eval_selection_stat(struct syntax_node *root);
int eval_while_stat(struct syntax_node *root);
int eval_do_while_stat(struct syntax_node *root);
int eval_for_stat(struct syntax_node *root);
int eval_jump_stat(struct syntax_node *root);
int eval_print_stat(struct syntax_node *root);
int eval_expression(struct syntax_node *root);
int eval_assignment_exp(struct syntax_node *root);
int eval_equality_exp(struct syntax_node *root);
int eval_relational_exp(struct syntax_node *root);
int eval_additive_exp(struct syntax_node *root);
int eval_mult_exp(struct syntax_node *root);
int eval_unary_exp(struct syntax_node *root);
int eval_postfix_exp(struct syntax_node *root);
int eval_primary_exp(struct syntax_node *root);

#endif
