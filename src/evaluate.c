#include <stdio.h>
#include <stdlib.h>
#include "evaluate.h"

extern struct token tokens[];

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
    printf("stat line : %ld\n", tokens[root->token_idx].line);
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
    return 0;
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
    printf("execute line : %ld\n", tokens[root->token_idx].line);
    return 0;
}

int eval_relational_exp(struct syntax_node *root)
{
    return 0;
}

int eval_additive_exp(struct syntax_node *root)
{
    return 0;
}

int eval_mult_exp(struct syntax_node *root)
{
    return 0;
}

int eval_unary_exp(struct syntax_node *root)
{
    return 0;
}

int eval_postfix_exp(struct syntax_node *root)
{
    return 0;
}

int eval_primary_exp(struct syntax_node *root)
{
    return 0;
}

