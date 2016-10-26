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

int main(int argc, char const *argv[])
{
    token_regex_init();

    char *code = read_code("input.txt");

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

    for (size_t i = 0; i < token_count; i++) {
        printf("token: %d\t\"", tokens[i].type);
        for (size_t j = 0; j < tokens[i].length; j++)
            putchar(tokens[i].literal[j]);
        printf("\"\t%ld\n", tokens[i].line);
    }

    printf("token_count = %ld\n", token_count);

    free(code);

    return 0;
}
