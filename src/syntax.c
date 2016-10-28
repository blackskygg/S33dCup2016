#include "syntax.h"
#include <stdio.h>
#include <stdlib.h>

#define check(t) (tokens[*idx].type == t)
#define consume(t) do { if (tokens[*idx].type == t) (*idx)++; } while (0)
#define malloc_node() ((struct syntax_node *)malloc(sizeof(struct syntax_node)))

extern struct token tokens[65536];

struct syntax_node *generate_ast()
{
    size_t iter = 0;
    struct syntax_node *root = stat_list(&iter);

    return root;
}

void destory_ast(struct syntax_node *root)
{
    if (root) {
        destory_ast(root->children);
        destory_ast(root->sibling);
        free(root);
    }
}

void print_ast(struct syntax_node *root, size_t level)
{
    if (root) {
        struct token *t = tokens + root->token_idx;

        for (size_t i = 0; i < level; i++)
            putchar('\t');
        printf("(%d) [%d] \"", root->type, t->type);
        for (size_t i = 0; i < t->length; i++)
            putchar(t->literal[i]);
        printf("\"\n");

        print_ast(root->children, level + 1);
        print_ast(root->sibling, level);
    }
}

struct syntax_node *stat_list(size_t *idx)
{
    if (check(END))
        return NULL;

    struct syntax_node *head = malloc_node();

    head->type = STAT_LIST;
    head->token_idx = *idx;
    head->children = stat(idx);

    struct syntax_node *iter = head->children;
    while (iter) {
        iter->sibling = stat(idx);
        iter = iter->sibling;
    }

    head->sibling = NULL;

    return head;
}

struct syntax_node *stat(size_t *idx)
{
    switch (tokens[*idx].type) {
    case DECL: return decl_stat(idx);
    case LBRACE: return compound_stat(idx);
    case IF: return selection_stat(idx);
    case WHILE: return while_stat(idx);
    case DO: return do_while_stat(idx);
    case FOR: return for_stat(idx);
    case BREAK: return jump_stat(idx);
    case PRINTF: return print_stat(idx);
    case END: return NULL;
    default: return exp_stat(idx);
    }
}

struct syntax_node *decl_stat(size_t *idx)
{
    if (!check(DECL))
        return NULL;

    struct syntax_node *node = malloc_node();

    node->type = SYN_DECL;
    node->token_idx = *idx;

    consume(DECL);

    node->children = init_decl_list(idx);

    consume(SEMI_COLON);

    node->sibling = NULL;

    return node;
}

struct syntax_node *init_decl_list(size_t *idx)
{
    if (!check(ID))
        return NULL;

    struct syntax_node *head = malloc_node();

    head->type = INIT_DECL_LIST;
    head->token_idx = *idx;
    head->children = init_decl(idx);

    struct syntax_node *iter = head->children;
    while (iter) {
        if (check(COMMA)) {
            consume(COMMA);
            iter->sibling = init_decl(idx);
            iter = iter->sibling;
        } else {
            iter->sibling = NULL;
            break;
        }
    }

    head->sibling = NULL;

    return head;
}

struct syntax_node *init_decl(size_t *idx)
{
    if (!check(ID))
        return NULL;

    struct syntax_node *node = malloc_node();
    node->type = INIT_DECL;
    node->token_idx = *idx;

    node->children = malloc_node();
    node->children->type = SYN_ID;
    node->children->token_idx = *idx;
    node->children->children = NULL;

    consume(ID);

    if (check(ASSIGN)) {
        consume(ASSIGN);
        node->children->sibling = expression(idx);
    } else {
        node->children->sibling = NULL;
    }

    node->sibling = NULL;

    return node;
}

struct syntax_node *exp_stat(size_t *idx)
{
    if (check(ID) || check(INT_CONST) ||
        check(ADD) || check(SUB) ||
        check(SEMI_COLON)) {

        struct syntax_node *node = malloc_node();
        node->type = EXP_STAT;
        node->token_idx = *idx;

        if (check(SEMI_COLON))
            node->children = NULL;
        else
            node->children = expression(idx);

        node->sibling = NULL;

        consume(SEMI_COLON);

        return node;
    } else {
        return NULL;
    }
}

struct syntax_node *compound_stat(size_t *idx)
{
    if (!check(LBRACE))
        return NULL;

    struct syntax_node *node = malloc_node();
    node->type = COMPOUND_STAT;
    node->token_idx = *idx;

    consume(LBRACE);

    node->children = stat_list(idx);

    consume(RBRACE);

    node->sibling = NULL;

    return node;
}

struct syntax_node *selection_stat(size_t *idx)
{
    return NULL;
}

struct syntax_node *while_stat(size_t *idx)
{
    return NULL;
}

struct syntax_node *do_while_stat(size_t *idx)
{
    return NULL;
}

struct syntax_node *for_stat(size_t *idx)
{
    return NULL;
}

struct syntax_node *jump_stat(size_t *idx)
{
    return NULL;
}

struct syntax_node *print_stat(size_t *idx)
{
    return NULL;
}

struct syntax_node *argument_list(size_t *idx)
{
    return NULL;
}

struct syntax_node *expression(size_t *idx)
{
    return NULL;
}

