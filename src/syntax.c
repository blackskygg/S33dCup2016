#include "syntax.h"
#include <stdio.h>
#include <stdlib.h>

#define check(t) (tokens[*idx].type == t)
#define consume() do { (*idx)++; } while (0)
#define malloc_node() ((struct syntax_node *)malloc(sizeof(struct syntax_node)))

extern struct token tokens[];

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
        printf("\"");
        for (size_t i = 0; i < t->length; i++)
            putchar(t->literal[i]);
        printf("\" ");
        printf("<%ld>\n", t->line);

        print_ast(root->children, level + 1);
        print_ast(root->sibling, level);
    }
}

struct syntax_node *stat_list(size_t *idx)
{
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
    case END: case RBRACE: return NULL;
    default: return exp_stat(idx);
    }
}

struct syntax_node *decl_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_DECL;
    node->token_idx = *idx;

    consume(); // int

    node->children = init_decl_list(idx);

    consume(); // ;

    node->sibling = NULL;

    return node;
}

struct syntax_node *init_decl_list(size_t *idx)
{
    if (!check(ID))
        return NULL;

    struct syntax_node *head = init_decl(idx);

    struct syntax_node *iter = head;
    while (iter && check(COMMA)) {
        consume(); // ,

        iter->sibling = init_decl(idx);
        iter = iter->sibling;
    }

    return head;
}

struct syntax_node *init_decl(size_t *idx)
{
    if (!check(ID))
        return NULL;

    struct syntax_node *node = malloc_node();
    node->type = SYN_INIT_DECL;
    node->token_idx = *idx;

    node->children = primary_exp(idx);

    if (check(ASSIGN)) {
        consume(); // =
        node->children->sibling = assignment_exp(idx);
    } else {
        node->children->sibling = NULL;
    }

    node->sibling = NULL;

    return node;
}

struct syntax_node *exp_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_EXP_STAT;

    if (check(SEMI_COLON))
        node->children = NULL;
    else
        node->children = expression(idx);

    node->token_idx = *idx;
    consume(); // ;

    node->sibling = NULL;

    return node;
}

struct syntax_node *compound_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_COMPOUND_STAT;
    node->token_idx = *idx;

    consume(); // {

    node->children = stat_list(idx);

    consume(); // }

    node->sibling = NULL;

    return node;
}

struct syntax_node *selection_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_SELECTION_STAT;
    node->token_idx = *idx;

    consume();  // if
    consume();  // (

    node->children = malloc_node();
    struct syntax_node *cond = node->children;
    cond->type = SYN_COND;
    cond->token_idx = *idx;
    cond->children = expression(idx);

    consume(); // )

    cond->sibling = malloc_node();
    struct syntax_node *body = cond->sibling;
    body->type = SYN_BODY;
    body->token_idx = *idx;
    body->children = stat(idx);

    if (check(ELSE)) {
        consume(); // else

        body->sibling = malloc_node();
        struct syntax_node *else_body = body->sibling;
        else_body->type = SYN_ELSE;
        else_body->token_idx = *idx;
        else_body->children = stat(idx);
        else_body->sibling = NULL;
    } else {
        body->sibling = NULL;
    }

    node->sibling = NULL;

    return node;
}

struct syntax_node *while_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_WHILE_STAT;
    node->token_idx = *idx;

    consume();  // while
    consume();  // (

    node->children = malloc_node();
    struct syntax_node *cond = node->children;
    cond->type = SYN_COND;
    cond->token_idx = *idx;
    cond->children = expression(idx);

    consume(); // )

    cond->sibling = malloc_node();
    struct syntax_node *body = cond->sibling;
    body->type = SYN_BODY;
    body->token_idx = *idx;
    body->children = stat(idx);

    body->sibling = NULL;

    node->sibling = NULL;
    return node;
}

struct syntax_node *do_while_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_DO_WHILE_STAT;
    node->token_idx = *idx;

    consume(); // do

    node->children = malloc_node();
    struct syntax_node *body = node->children;
    body->type = SYN_BODY;
    body->token_idx = *idx;
    body->children = stat(idx);

    consume();  // while

    consume();  // (

    body->sibling = malloc_node();
    struct syntax_node *cond = body->sibling;
    cond->type = SYN_COND;
    cond->token_idx = *idx;
    cond->children = expression(idx);

    consume(); // )

    cond->sibling = NULL;

    consume(); // ;

    node->sibling = NULL;
    return node;
}

struct syntax_node *for_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_FOR_STAT;
    node->token_idx = *idx;

    consume();  // for
    consume();  // (

    node->children = malloc_node();
    struct syntax_node *bootstrap = node->children;
    bootstrap->type = SYN_BOOTSTRAP;
    bootstrap->token_idx = *idx;
    bootstrap->children = check(DECL) ? init_decl_list(idx) : expression(idx);

    consume(); // ;

    bootstrap->sibling = malloc_node();
    struct syntax_node *cond = bootstrap->sibling;
    cond->type = SYN_COND;
    cond->token_idx = *idx;
    cond->children = expression(idx);

    consume(); // ;

    cond->sibling = malloc_node();
    struct syntax_node *stepin = cond->sibling;
    stepin->type = SYN_STEPIN;
    stepin->token_idx = *idx;
    stepin->children = expression(idx);

    consume(); // )

    stepin->sibling = malloc_node();
    struct syntax_node *body = stepin->sibling;
    body->type = SYN_BODY;
    body->token_idx = *idx;
    body->children = stat(idx);

    body->sibling = NULL;

    node->sibling = NULL;

    return node;
}

struct syntax_node *jump_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_JUMP_STAT;
    node->token_idx = *idx;

    consume(); // break

    node->children = NULL;

    consume(); // ;

    node->sibling = NULL;

    return node;
}

struct syntax_node *print_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_PRINT_STAT;
    node->token_idx = *idx;

    consume();  // printf
    consume();  // (

    node->children = expression(idx);

    consume();  // )
    consume();  // ;

    node->sibling = NULL;

    return node;
}

struct syntax_node *expression(size_t *idx)
{
    struct syntax_node *head = assignment_exp(idx);

    struct syntax_node *iter = head;

    while (iter && check(COMMA)) {
        consume(); // ,

        iter->sibling = assignment_exp(idx);
        iter = iter->sibling;
    }

    return head;
}

struct syntax_node *assignment_exp(size_t *idx)
{
    if (tokens[*idx].type != ID || tokens[*idx + 1].type != ASSIGN)
        return rotate_exp(equality_exp(idx));

    struct syntax_node *node = malloc_node();
    node->type = SYN_ASSIGNMENT_EXP;
    node->children = primary_exp(idx);
    node->token_idx = *idx;

    consume(); // ID

    node->children->sibling = assignment_exp(idx);
    node->sibling = NULL;

    return node;
}

struct syntax_node *equality_exp(size_t *idx) // L_comb
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_EQUALITY_EXP;
    node->children = rotate_exp(relational_exp(idx));
    node->token_idx = *idx;

    if (check(EQ) || check(NE)) {
        consume(); // == | !=

        node->children->sibling = equality_exp(idx);
        node->sibling = NULL;

        return node;
    } else {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    }
}

struct syntax_node *relational_exp(size_t *idx) // L_comb
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_RELATIONAL_EXP;
    node->children = rotate_exp(additive_exp(idx));
    node->token_idx = *idx;

    if (check(GT) || check(LT) || check(GE) || check(LE)) {
        consume(); // > | < | >= | <=

        node->children->sibling = relational_exp(idx);
        node->sibling = NULL;

        return node;
    } else {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    }
}


struct syntax_node *additive_exp(size_t *idx) // L_comb
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_ADDITIVE_EXP;
    node->children = rotate_exp(mult_exp(idx));
    node->token_idx = *idx;

    if (check(ADD) || check(SUB)) {
        consume(); // + | -

        node->children->sibling = additive_exp(idx);
        node->sibling = NULL;

        return node;
    } else {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    }
}

struct syntax_node *mult_exp(size_t *idx) // L_comb
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_MULT_EXP;
    node->children = unary_exp(idx);
    node->token_idx = *idx;

    if (check(MUL) || check(DIV)) {
        consume(); // * | /

        node->children->sibling = mult_exp(idx);
        node->sibling = NULL;

        return node;
    } else {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    }
}

struct syntax_node *unary_exp(size_t *idx)
{
    struct syntax_node *root;

    struct syntax_node **iter = &root;

    while (check(ADD) || check(SUB)) {
        *iter = malloc_node();
        (*iter)->type = SYN_UNARY_EXP;
        (*iter)->token_idx = *idx;

        consume(); // + | -

        (*iter)->children = NULL;
        (*iter)->sibling = NULL;
        iter = &((*iter)->children);
    }

    *iter = postfix_exp(idx);

    return root;
}

struct syntax_node *postfix_exp(size_t *idx)
{
    struct syntax_node *root = primary_exp(idx);

    while (root && (check(INC) || check(DEC))) {
        struct syntax_node *new_root = malloc_node();
        new_root->type = SYN_POSTFIX_EXP;
        new_root->token_idx = *idx;

        consume(); // ++ | --

        new_root->children = root;
        new_root->sibling = NULL;

        root = new_root;
    }

    return root;
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
    } else if (check(STRING)) {
        node = malloc_node();
        node->type = SYN_STRING;
        node->token_idx = *idx;

        consume();

        node->children = NULL;
        node->sibling = NULL;
    }
    return node;
}

struct syntax_node *rotate_exp(struct syntax_node *root)
{
    if (!root || !root->children)
        return root;
    struct syntax_node *left = root->children->sibling;
    if (left == NULL || left->type != root->type) {
        return root;
    } else {
        struct syntax_node *tmp = left->children;
        left->children = root;
        root->sibling = tmp->sibling;
        tmp->sibling = NULL;
        root->children->sibling = tmp;
        return rotate_exp(left);
    }
}
