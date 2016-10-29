#include <stdio.h>
#include <stdlib.h>
#include "evaluate.h"

extern struct token tokens[];

int prev_line = -1;

struct scope_record *scope = NULL;

#define malloc_record() ((struct scope_record *)malloc(sizeof(struct scope_record)))

struct scope_record *enter_scope()
{
    struct scope_record *new_head = malloc_record();
    new_head->is_scope_flag = 1;
    new_head->key = NULL;
    new_head->len = 0;
    new_head->val = 0;
    new_head->prev = scope;
    return new_head;
}

struct scope_record *leave_scope()
{
    struct scope_record *new_head = NULL;

    if (scope) {
        while (scope && !scope->is_scope_flag) {
            new_head = scope->prev;
            free(scope);
            scope = new_head;
        }
        new_head = scope->prev;
        free(scope);
    }

    return new_head;
}

int keycmp(char *k1, size_t l1, char *k2, size_t l2)
{
    if (l1 != l2 || !k1 || !k2) 
        return 0;

    for (size_t i = 0; i < l1; ++i) {
        if (k1[i] != k2[i])
            return 0;
    }

    return 1;
}

struct scope_record *new_record(char *key, size_t len, int val)
{
    struct scope_record *new_head = malloc_record();
    new_head->is_scope_flag = 0;
    new_head->key = key;
    new_head->len = len;
    new_head->val = val;
    new_head->prev = scope;
    return new_head;
}

int get_record(char *key, size_t len)
{
    struct scope_record *iter = scope;
    while (iter && !keycmp(iter->key, iter->len, key, len))
        iter = iter->prev;
    return iter->val;
}

void set_record(char *key, size_t len, int val)
{
    struct scope_record *iter = scope;
    while (iter && !keycmp(iter->key, iter->len, key, len))
        iter = iter->prev;
    iter->val = val;
    return;
}

int eval_stat_list(struct syntax_node *root)
{
    struct syntax_node *iter = root->children;

    while (iter) {
        eval_stat(iter);
        iter = iter->sibling;
    }

    return 0;
}

int eval_stat(struct syntax_node *root)
{
    switch (root->type) {
        case SYN_DECL: return eval_decl_stat(root);
        case SYN_COMPOUND_STAT: return eval_compound_stat(root);
        case SYN_SELECTION_STAT: return eval_selection_stat(root);
        case SYN_WHILE_STAT: return eval_while_stat(root);
        case SYN_DO_WHILE_STAT: return eval_while_stat(root);
        case SYN_FOR_STAT: return eval_for_stat(root);
        case SYN_JUMP_STAT: return eval_jump_stat(root);
        case SYN_PRINT_STAT: return eval_print_stat(root);
        default: return eval_exp_stat(root);
    }
}

int eval_decl_stat(struct syntax_node *root)
{
    return eval_init_decl_list(root->children);
}

int eval_init_decl_list(struct syntax_node *root)
{
    int result = 0;
    struct syntax_node *iter = root;
    while (iter) {
        result = eval_init_decl(iter);
        iter = iter->sibling;
    }
    return result;
}

int eval_init_decl(struct syntax_node *root)
{
    struct token *t = tokens + root->children->token_idx;
    char *key = t->literal;
    size_t len = t->length;
    int val = root->children->sibling ? eval_expression(root->children->sibling) : 0;
    scope = new_record(key, len, val);

    return 0;
}

int eval_exp_stat(struct syntax_node *root)
{
    if (root->children) {
        return eval_expression(root->children);
    } else {
        size_t line = tokens[root->token_idx].line;
        if (line != prev_line) {
            printf("empty stat : %ld\n", line);
            prev_line = line;
        }

        return 0;
    }
}

int eval_compound_stat(struct syntax_node *root)
{
    return eval_stat_list(root->children);
}

int eval_selection_stat(struct syntax_node *root)
{
    return 0;
}

int eval_while_stat(struct syntax_node *root)
{
    return 0;
}

int eval_do_while_stat(struct syntax_node *root)
{
    return 0;
}

int eval_for_stat(struct syntax_node *root)
{
    return 0;
}

int eval_jump_stat(struct syntax_node *root)
{
    return 0;
}

int eval_print_stat(struct syntax_node *root)
{

    return eval_expression(root->children);
}


int eval_expression(struct syntax_node *root)
{
    int result = 0;
    struct syntax_node *iter = root;
    while (iter) {
        result = eval_assignment_exp(iter);
        iter = iter->sibling;
    }
    return result;
}

// line_printer
int eval_assignment_exp(struct syntax_node *root)
{
    if (root->type != SYN_ASSIGNMENT_EXP)
        return eval_equality_exp(root);

    struct token *t = tokens + root->children->token_idx;
    char *key = t->literal;
    size_t len = t->length;
    int val = eval_assignment_exp(root->children->sibling);

    set_record(key, len, val);

    return val;
}

int eval_equality_exp(struct syntax_node *root)
{
    int res = 0;

    size_t line = tokens[root->token_idx].line;
    if (line != prev_line) {
        printf("eval line : %ld\n", line);
        prev_line = line;
    }

    if (root->type != SYN_EQUALITY_EXP) {
        res = eval_relational_exp(root);
    } else {
        struct token *t = tokens + root->token_idx;
        struct syntax_node *exp1 = root->children, *exp2 = root->children->sibling;

        if (t->type == EQ)
            res =  eval_equality_exp(exp1) == eval_relational_exp(exp2); 
        else if (t->type == NE)
            res =  eval_equality_exp(exp1) != eval_relational_exp(exp2); 
    }

    printf("(%d)\n", res);

    return res;
}

int eval_relational_exp(struct syntax_node *root)
{
    int res = 0;

    if (root->type != SYN_RELATIONAL_EXP) {
        res = eval_additive_exp(root);
    } else {
        struct token *t = tokens + root->token_idx;
        struct syntax_node *exp1 = root->children, *exp2 = root->children->sibling;

        if (t->type == LT)
            res =  eval_relational_exp(exp1) < eval_additive_exp(exp2); 
        else if (t->type == GT)
            res =  eval_relational_exp(exp1) > eval_additive_exp(exp2); 
        else if (t->type == LE)
            res =  eval_relational_exp(exp1) <= eval_additive_exp(exp2); 
        else if (t->type == GE)
            res =  eval_relational_exp(exp1) >= eval_additive_exp(exp2); 
    }

    return res;
}

int eval_additive_exp(struct syntax_node *root)
{
    int res = 0;

    if (root->type != SYN_ADDITIVE_EXP) {
        res = eval_mult_exp(root);
    } else {
        struct token *t = tokens + root->token_idx;
        struct syntax_node *exp1 = root->children, *exp2 = root->children->sibling;

        if (t->type == ADD)
            res =  eval_additive_exp(exp1) + eval_mult_exp(exp2); 
        else if (t->type == SUB)
            res =  eval_additive_exp(exp1) - eval_mult_exp(exp2); 
    }

    return res;
}

int eval_mult_exp(struct syntax_node *root)
{
    int res = 0;

    if (root->type != SYN_MULT_EXP) {
        res = eval_unary_exp(root);
    } else {
        struct token *t = tokens + root->token_idx;
        struct syntax_node *exp1 = root->children, *exp2 = root->children->sibling;

        if (t->type == MUL)
            res =  eval_mult_exp(exp1) * eval_unary_exp(exp2); 
        else if (t->type == DIV)
            res =  eval_mult_exp(exp1) / eval_unary_exp(exp2); 
    }

    return res;
}

int eval_unary_exp(struct syntax_node *root)
{
    int res = 0;

    if (root->type != SYN_UNARY_EXP) {
        res = eval_postfix_exp(root);
    } else {
        struct token *t = tokens + root->token_idx;
        struct syntax_node *exp = root->children;

        if (t->type == ADD)
            res = +eval_unary_exp(exp);
        else if (t->type == SUB)
            res = -eval_unary_exp(exp);
    }

    return res;
}

int eval_postfix_exp(struct syntax_node *root)
{
    int res = 0;

    if (root->type != SYN_POSTFIX_EXP) {
        res = eval_primary_exp(root);
    } else {
        struct token *t = tokens + root->token_idx;

        struct syntax_node *exp = root->children;
        struct token *t_id = tokens + exp->token_idx;
        res = get_record(t_id->literal, t_id->length);

        if (t->type == INC)
            set_record(t_id->literal, t_id->length, res + 1);
        else if (t->type == DEC)
            set_record(t_id->literal, t_id->length, res - 1);
    }

    return res;
}

int eval_primary_exp(struct syntax_node *root)
{
    int res = 0;
    struct token *t = tokens + root->token_idx;

    if (root->type == SYN_ID) {
        res = get_record(t->literal, t->length);
    } else if (root->type == SYN_INT_CONST) {
        char tmp = t->literal[t->length];
        t->literal[t->length] = '\0';

        res = atoi(t->literal);

        t->literal[t->length] = tmp;
    }

    return res;
}

