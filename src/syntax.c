#include "syntax.h"
#include <stdio.h>
#include <stdlib.h>

#define check(t) (tokens[*idx].type == t)
#define consume() do { (*idx)++; } while (0)
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
        printf("(%d) [%ld] ", root->type, root->token_idx);
        // if (t->type == ID || t->type == STRING || t->type == INT_CONST) {
            printf("\"");
            for (size_t i = 0; i < t->length; i++)
                putchar(t->literal[i]);
            printf("\" ");
        // }
        printf("<%ld>\n", t->line);

        print_ast(root->children, level + 1);
        print_ast(root->sibling, level);
    }
}

struct syntax_node *stat_list(size_t *idx)
{
    if (check(END))
        return NULL;

    struct syntax_node *head = malloc_node();

    head->type = SYN_STAT_LIST;
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

    consume();

    node->children = init_decl_list(idx);

    consume();

    node->sibling = NULL;

    return node;
}

struct syntax_node *init_decl_list(size_t *idx)
{
    if (!check(ID))
        return NULL;

    struct syntax_node *head = malloc_node();

    head->type = SYN_INIT_DECL_LIST;
    head->token_idx = *idx;
    head->children = init_decl(idx);

    struct syntax_node *iter = head->children;
    while (iter) {
        if (check(COMMA)) {
            consume();
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
    node->type = SYN_INIT_DECL;
    node->token_idx = *idx;

    node->children = malloc_node();
    node->children->type = SYN_ID;
    node->children->token_idx = *idx;
    node->children->children = NULL;

    consume();

    if (check(ASSIGN)) {
        consume();
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
        node->type = SYN_EXP_STAT;
        node->token_idx = *idx;

        if (check(SEMI_COLON))
            node->children = NULL;
        else
            node->children = expression(idx);

        consume();

        node->sibling = NULL;

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
    node->type = SYN_COMPOUND_STAT;
    node->token_idx = *idx;

    consume();

    node->children = stat_list(idx);

    consume();

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
    if (!check(BREAK))
        return NULL;
    struct syntax_node *node = malloc_node();
    node->type = SYN_JUMP_STAT;
    node->token_idx = *idx;

    consume();

    node->children = NULL;

    consume();

    node->sibling = NULL;

    return node;
}

struct syntax_node *print_stat(size_t *idx)
{
    return NULL;
}

struct syntax_node *argument_list(size_t *idx)
{
    return NULL;
}

struct syntax_node *assignment_exp(size_t *idx);
struct syntax_node *equality_exp(size_t *idx);
struct syntax_node *relational_exp(size_t *idx);
struct syntax_node *additive_exp(size_t *idx);
struct syntax_node *mult_exp(size_t *idx);
struct syntax_node *unary_exp(size_t *idx);
struct syntax_node *postfix_exp(size_t *idx);
struct syntax_node *primary_exp(size_t *idx);

struct syntax_node *expression(size_t *idx)
{
    return equality_exp(idx);
}

struct syntax_node *assignment_exp(size_t *idx)
{
    /*
    struct syntax_node *node = malloc_node();
    node->type = SYN_ASSIGNMENT_EXP;
    node->children = primary_exp(idx);
    node->token_idx = *idx;

    if (check(ASSIGN)) {
        consume();

        node->sibling = assignment_exp(idx);

        return node;
    } else {
        struct syntax_node * result = node->children;
        free(node);
        return result;
    }
    */
    return NULL;
}

struct syntax_node *equality_exp(size_t *idx)
{
    struct syntax_node *node = malloc_node();
    node->type = SYN_EQUALITY_EXP;
    node->children = relational_exp(idx);
    node->token_idx = *idx;

    if (check(EQ) || check(NE)) {
        consume();

        node->sibling = equality_exp(idx);

        return node;
    } else {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    }
}

struct syntax_node *relational_exp(size_t *idx)
{
    struct syntax_node *node = malloc_node();
    node->type = SYN_RELATIONAL_EXP;
    node->children = additive_exp(idx);
    node->token_idx = *idx;

    if (check(GT) || check(LT) || check(GE) || check(LE)) {
        consume();

        node->sibling = relational_exp(idx);

        return node;
    } else {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    }
}


struct syntax_node *additive_exp(size_t *idx)
{
    struct syntax_node *node = malloc_node();
    node->type = SYN_ADDITIVE_EXP;
    node->children = mult_exp(idx);
    node->token_idx = *idx;

    if (check(ADD) || check(SUB)) {
        consume();

        node->sibling = additive_exp(idx);

        return node;
    } else {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    }
}

struct syntax_node *mult_exp(size_t *idx)
{
    struct syntax_node *node = malloc_node();
    node->type = SYN_MULT_EXP;
    node->children = unary_exp(idx);
    node->token_idx = *idx;

    if (check(MUL) || check(DIV)) {
        consume();

        node->sibling = mult_exp(idx);

        return node;
    } else {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    }
}

struct syntax_node *unary_exp(size_t *idx)
{
    struct syntax_node *node = malloc_node();
    node->type = SYN_UNARY_EXP;
    node->token_idx = *idx;

    struct syntax_node **iter = &(node->children);
    while (check(ADD) || check(SUB)) {
        *iter = malloc_node();
        (*iter)->type = check(ADD) ? SYN_POS : SYN_NEG;
        (*iter)->token_idx = *idx;

        consume();

        (*iter)->children = NULL;
        (*iter)->sibling = NULL;
        iter = &((*iter)->sibling);
    }

    *iter = postfix_exp(idx);

    if (*iter == node->children) {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    } else {
        node->sibling = NULL;
        return node;
    }
}

struct syntax_node *postfix_exp(size_t *idx)
{
    struct syntax_node *node = malloc_node();
    node->type = SYN_POSTFIX_EXP;
    node->token_idx = *idx;
    node->children = primary_exp(idx);

    struct syntax_node *iter = node->children;
    while (iter && (check(INC) || check(DEC))) {
        iter->sibling = malloc_node();
        iter->sibling->type = check(INC) ? SYN_INC : SYN_DEC;
        iter->sibling->token_idx = *idx;

        consume();

        iter->sibling->children = NULL;
        iter->sibling->sibling = NULL;
        iter = iter->sibling;
    }

    if (node->children->sibling) {
        node->sibling = NULL;
        return node;
    } else {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    }
}

struct syntax_node *primary_exp(size_t *idx)
{
    struct syntax_node *node = NULL;
    if (check(ID)) {
        node = malloc_node();
        node->type = SYN_ID;
        node->token_idx = *idx;
        
        consume();

        node->children = NULL;
        node->sibling = NULL;
    } else if (check(INT_CONST)) {
        node = malloc_node();
        node->type = SYN_INT_CONST;
        node->token_idx = *idx;

        consume();

        node->children = NULL;
        node->sibling = NULL;
    }
    return node;
}
