#include "encode.h"
#include <stdlib.h>

char *token_type_string(struct token tokens[], size_t token_count)
{
    char *type_string = (char *)malloc(sizeof(char) * (token_count + 1));
    type_string[token_count] = '\0';
    for (size_t i = 0; i < token_count; i++) {
        char c;
        switch (tokens[i].type) {
            case INT_CONST: c = 'N'; break;
            case STRING:    c = 'S'; break;
            case DECL:      c = 'd'; break;
            case IF:        c = 'i'; break;
            case ELSE:      c = 'e'; break;
            case WHILE:     c = 'w'; break;
            case DO:        c = 'd'; break;
            case FOR:       c = 'f'; break;
            case BREAK:     c = 'b'; break;
            case COMMA:     c = ','; break;
            case SEMI_COLON:c = ';'; break;
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
        type_string[i] = c;
    }

    return type_string;
}
