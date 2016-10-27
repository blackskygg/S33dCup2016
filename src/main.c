#include <stdio.h>
#include <stdlib.h>
#include "token.h"

struct token tokens[65536];

char *read_code(const char *fname)
{
    FILE *fcode = fopen(fname, "rb");

    fseek(fcode, 0, SEEK_END);
    size_t fsize = ftell(fcode);
    rewind(fcode);

    char *buf = (char *)malloc(sizeof(char) * (fsize + 1));
    fread(buf, 1, fsize, fcode);
    buf[fsize] = '\0';

    return buf;
}

void print_type_string(struct token tokens[], size_t token_count);

int main(int argc, char const *argv[])
{
    token_regex_init();

    // MALLOC code
    char *code = read_code("input.txt");

    // fill tokens (not SP, CRLF or COMMENT) into tokens[]
    // store tokens[] length into token_count
    char *iter = code;
    size_t token_count = 0;
    while (*iter && *iter != EOF) {
        struct token t = token_scan(iter);
        iter += t.length;

        if (t.type == SP || t.type == CRLF || t.type == COMMENT)
            continue;
        tokens[token_count] = t;
        token_count++;
    }

    // DEBUG display
    for (size_t i = 0; i < token_count; i++) {
        printf("token: %d\t\"", tokens[i].type);
        for (size_t j = 0; j < tokens[i].length; j++)
            putchar(tokens[i].literal[j]);
        printf("\"\t%ld\n", tokens[i].line);
    }
    printf("token_count = %ld\n", token_count);

    // DEBUG display
    print_type_string(tokens, token_count);

    // FREE code
    free(code);

    return 0;
}

void print_type_string(struct token tokens[], size_t token_count)
{
    for (size_t i = 0; i < token_count; i++) {
        char c;
        switch (tokens[i].type) {
        case INT_CONST: c = 'N'; break;
        case STRING:    c = 'S'; break;
        case INT_DECL:  c = 'd'; break;
        case IF:        c = 'i'; break;
        case ELSE:      c = 'e'; break;
        case WHILE:     c = 'w'; break;
        case DO:        c = 'd'; break;
        case FOR:       c = 'f'; break;
        case BREAK:     c = 'b'; break;
        case PRINTF:    c = 'p'; break;
        case COMMA:     c = ','; break;
        case SEMI_COLON: c = ';'; break;
        case LBRACE:    c = '{'; break;
        case RBRACE:    c = '}'; break;
        case LPAREN:    c = '('; break;
        case RPAREN:    c = ')'; break;
        case EQ:        c = '='; break;
        case NE:        c = '~'; break;
        case LE:        c = 'l'; break;
        case GE:        c = 'g'; break;
        case LT:        c = '<'; break;
        case GT:        c = '>'; break;
        case ASSIGN:    c = ':'; break;
        case INC:       c = '^'; break;
        case DEC:       c = 'v'; break;
        case ADD:       c = '+'; break;
        case SUB:       c = '-'; break;
        case MUL:       c = '*'; break;
        case DIV:       c = '/'; break;
        case ID:        c = 'I'; break;
        default:        c = '\0'; break;
        }
        putchar(c);
    }
    putchar('\n');
    return;
}
