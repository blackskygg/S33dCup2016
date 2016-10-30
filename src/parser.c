#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

// 用来写状态机的辅助宏（其实就是短函数）
#define check(t) (tokens[*idx].type == t)                                                                   // 检查这个token类型
#define consume(t) \
    do { \
        if (!check(t)) { \
            destory_ast(node); \
            return NULL; \
        } else { \
            (*idx)++; \
        } \
    } while (0)                                 // 检查并消耗一个token
#define do_consume() do { (*idx)++; } while (0) // 消耗一个token

// malloc语法树结点的辅助函数，malloc实在是太长了
struct syntax_node *malloc_node()
{
    struct syntax_node *result = (struct syntax_node *)malloc(sizeof(struct syntax_node));

    result->type = (enum syntax_type)(-1);
    result->token_idx = 0;
    result->children = NULL;
    result->sibling = NULL;

    return result;
}

// 生成语法树包装后的启动函数，提供一个启动的iter供内部所有函数用（相当于一个闭包？）
struct syntax_node *generate_ast()
{
    size_t iter = 0;
    // 在本题中全局的语法结点类型是stat_list
    struct syntax_node *node = stat_list(&iter);

    return node;
}

// 。。。这个应该不用注释了吧
void destory_ast(struct syntax_node *node)
{
    if (node) {
        destory_ast(node->children);
        destory_ast(node->sibling);
        free(node);
    }
}

// 。。。这个也应该不用注释了吧
void print_ast(struct syntax_node *node, size_t level)
{
    if (node) {
        struct token *t = tokens + node->token_idx;

        for (size_t i = 0; i < level; i++)
            putchar('\t');
        printf("(%d) [%ld] ", node->type, node->token_idx);
        printf("\"");
        for (size_t i = 0; i < t->length; i++)
            putchar(t->literal[i]);
        printf("\" ");
        printf("<%ld>\n", t->line);

        print_ast(node->children, level + 1);
        print_ast(node->sibling, level);
    }
}

/*****************************************************************
* 请注意！！！！！！接下来是又臭又长的具体状态机实现，请看提供的文档。
* 当然你要是愿意读那我也没什么好说的。。。
* 简单地写了点注释给自己写程序的时候看的。
*****************************************************************/
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

    case SEMI_COLON:
    case ID:
    case INT_CONST:
    case ADD:
    case SUB:
        return exp_stat(idx);
    // dealing with stat_list's ending token -> next token

    default: return NULL;
    }
}

struct syntax_node *decl_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_DECL;
    node->token_idx = *idx;

    consume(INT_CONST);

    node->children = init_decl_list(idx);

    consume(SEMI_COLON);

    return node;
}

struct syntax_node *init_decl_list(size_t *idx)
{
    // TODO: fix
    if (!check(ID))
        return NULL;

    struct syntax_node *head = init_decl(idx);

    struct syntax_node *iter = head;
    while (iter && check(COMMA)) {
        do_consume();

        iter->sibling = init_decl(idx);
        iter = iter->sibling;
    }

    return head;
}

struct syntax_node *init_decl(size_t *idx)
{
    // TODO: fix
    if (!check(ID))
        return NULL;

    struct syntax_node *node = malloc_node();
    node->type = SYN_INIT_DECL;
    node->token_idx = *idx;

    node->children = primary_exp(idx);

    if (check(ASSIGN)) {
        consume(EQ);
        node->children->sibling = assignment_exp(idx);
    }

    return node;
}

struct syntax_node *exp_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_EXP_STAT;

    if (!check(SEMI_COLON))
        node->children = expression(idx);

    node->token_idx = *idx;
    consume(SEMI_COLON);

    return node;
}

struct syntax_node *compound_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_COMPOUND_STAT;
    node->token_idx = *idx;

    consume(LBRACE);

    node->children = stat_list(idx);

    consume(RBRACE);

    return node;
}

struct syntax_node *selection_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_SELECTION_STAT;
    node->token_idx = *idx;

    consume(IF);
    consume(LPAREN);

    node->children = malloc_node();
    struct syntax_node *cond = node->children;
    cond->type = SYN_COND;
    cond->token_idx = *idx;
    cond->children = expression(idx);

    consume(RPAREN);

    cond->sibling = malloc_node();
    struct syntax_node *body = cond->sibling;
    body->type = SYN_BODY;
    body->token_idx = *idx;
    body->children = stat(idx);

    body->sibling = malloc_node();
    struct syntax_node *else_body = body->sibling;
    else_body->type = SYN_ELSE;
    else_body->token_idx = *idx;

    if (check(ELSE)) {
        do_consume();

        else_body->children = stat(idx);
    }

    return node;
}

struct syntax_node *while_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_WHILE_STAT;
    node->token_idx = *idx;

    consume(WHILE);
    consume(LPAREN);

    node->children = malloc_node();
    struct syntax_node *cond = node->children;
    cond->type = SYN_COND;
    cond->token_idx = *idx;
    cond->children = expression(idx);

    consume(RPAREN);

    cond->sibling = malloc_node();
    struct syntax_node *body = cond->sibling;
    body->type = SYN_BODY;
    body->token_idx = *idx;
    body->children = stat(idx);

    return node;
}

struct syntax_node *do_while_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_DO_WHILE_STAT;
    node->token_idx = *idx;

    consume(DO);

    node->children = malloc_node();
    struct syntax_node *body = node->children;
    body->type = SYN_BODY;
    body->token_idx = *idx;
    body->children = stat(idx);

    consume(WHILE);

    consume(LBRACE);

    body->sibling = malloc_node();
    struct syntax_node *cond = body->sibling;
    cond->type = SYN_COND;
    cond->token_idx = *idx;
    cond->children = expression(idx);

    consume(RPAREN);

    consume(SEMI_COLON);

    return node;
}

struct syntax_node *for_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_FOR_STAT;
    node->token_idx = *idx;

    consume(FOR);
    consume(LPAREN);

    node->children = malloc_node();
    struct syntax_node *bootstrap = node->children;
    bootstrap->type = SYN_BOOTSTRAP;
    bootstrap->token_idx = *idx;
    if (check(DECL)) {
        do_consume();
        bootstrap->children = init_decl_list(idx);
    } else {
        bootstrap->children = expression(idx);
    }

    consume(SEMI_COLON);

    bootstrap->sibling = malloc_node();
    struct syntax_node *cond = bootstrap->sibling;
    cond->type = SYN_COND;
    cond->token_idx = *idx;
    cond->children = expression(idx);

    consume(SEMI_COLON);

    cond->sibling = malloc_node();
    struct syntax_node *stepin = cond->sibling;
    stepin->type = SYN_STEPIN;
    stepin->token_idx = *idx;
    stepin->children = expression(idx);

    consume(RPAREN);

    stepin->sibling = malloc_node();
    struct syntax_node *body = stepin->sibling;
    body->type = SYN_BODY;
    body->token_idx = *idx;
    body->children = stat(idx);

    return node;
}

struct syntax_node *jump_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_JUMP_STAT;
    node->token_idx = *idx;

    consume(BREAK);

    consume(SEMI_COLON);

    return node;
}

struct syntax_node *print_stat(size_t *idx)
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_PRINT_STAT;
    node->token_idx = *idx;

    consume(PRINTF);
    consume(LPAREN);

    node->children = expression(idx);

    consume(RPAREN);
    consume(SEMI_COLON);

    return node;
}

struct syntax_node *expression(size_t *idx)
{
    struct syntax_node *head = assignment_exp(idx);

    struct syntax_node *iter = head;

    while (iter && check(COMMA)) {
        do_consume();

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

    consume(ID);

    node->children->sibling = assignment_exp(idx);

    return node;
}

struct syntax_node *equality_exp(size_t *idx) // L_comb
{
    struct syntax_node *node = malloc_node();

    node->type = SYN_EQUALITY_EXP;
    node->children = rotate_exp(relational_exp(idx));
    node->token_idx = *idx;

    if (check(EQ) || check(NE)) {
        do_consume(); // == | !=

        node->children->sibling = equality_exp(idx);

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
        do_consume(); // > | < | >= | <=

        node->children->sibling = relational_exp(idx);

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
        do_consume(); // + | -

        node->children->sibling = additive_exp(idx);

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
        do_consume(); // * | /

        node->children->sibling = mult_exp(idx);

        return node;
    } else {
        struct syntax_node *result = node->children;
        free(node);
        return result;
    }
}

struct syntax_node *unary_exp(size_t *idx)
{
    struct syntax_node *node;

    struct syntax_node **iter = &node;

    while (check(ADD) || check(SUB)) {
        *iter = malloc_node();
        (*iter)->type = SYN_UNARY_EXP;
        (*iter)->token_idx = *idx;

        do_consume(); // + | -

        iter = &((*iter)->children);
    }

    *iter = postfix_exp(idx);

    return node;
}

struct syntax_node *postfix_exp(size_t *idx)
{
    struct syntax_node *node = primary_exp(idx);

    if (node && (check(INC) || check(DEC))) {
        struct syntax_node *new_node = malloc_node();
        new_node->type = SYN_POSTFIX_EXP;
        new_node->token_idx = *idx;

        do_consume(); // ++ | --

        new_node->children = node;

        node = new_node;
    }

    return node;
}

struct syntax_node *primary_exp(size_t *idx)
{
    struct syntax_node *node = NULL;

    if (check(ID)) {
        node = malloc_node();
        node->type = SYN_ID;
        node->token_idx = *idx;

        do_consume(); // ID
    } else if (check(INT_CONST)) {
        node = malloc_node();
        node->type = SYN_INT_CONST;
        node->token_idx = *idx;

        do_consume(); // INT_CONST
    } else if (check(STRING)) {
        node = malloc_node();
        node->type = SYN_STRING;
        node->token_idx = *idx;

        do_consume();  // STRING
    }
    return node;
}

struct syntax_node *rotate_exp(struct syntax_node *node)
{
    if (!node || !node->children)
        return node;
    struct syntax_node *left = node->children->sibling;
    if (left == NULL || left->type != node->type) {
        return node;
    } else {
        struct syntax_node *tmp = left->children;
        left->children = node;
        node->sibling = tmp->sibling;
        node->children->sibling = tmp;
        return rotate_exp(left);
    }
}

#undef check
#undef consume
#undef do_consume
#undef malloc_node
