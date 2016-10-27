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
    struct syntax_node *root;

    switch (tokens[*idx].type) {
    case DECL: root = decl_stat(idx); break;
    case LBRACE: root = compound_stat(idx); break;
    case IF: root = selection_stat(idx); break;
    case WHILE: root = while_stat(idx); break;
    case DO: root = do_while_stat(idx); break;
    case FOR: root = for_stat(idx); break;
    case BREAK: root = jump_stat(idx); break;
    case PRINTF: root = print_stat(idx); break;
    default: root = exp_stat(idx); break;
    }
    return root;
}

struct syntax_node *decl_stat(size_t *idx)
{
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
    struct syntax_node *head = malloc_node();

    head->type = INIT_DECL_LIST;
    head->token_idx = *idx;
    head->children = init_decl(idx);

    struct syntax_node *iter = head->children;
    while (iter) {
        consume(COMMA);
        iter->sibling = init_decl(idx);
        iter = iter->sibling;
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
    consume(ID);
    node->children = NULL;
    node->sibling = NULL;
    return node;
}

struct syntax_node *exp_stat(size_t *idx)
{
    return NULL;
}
struct syntax_node *compound_stat(size_t *idx)
{
    return NULL;
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
