#include <stdio.h>
#include <stdlib.h>
#include "token.h"

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
    token_re_init();

    char *code = read_code("input.txt");

    char *iter = code;
    size_t token_count = 0;
    while (*iter && *iter != EOF) {
        struct token t = token_scan(iter);

        printf("token: %d\t\"", t.type);
        for (size_t i = 0; i < t.token_len; i++)
            putchar(t.token[i]);
        printf("\"\t%ld\t%ld\n", t.token_len, t.line_num);

        iter += t.token_len;
        token_count++;
    }

    printf("token_count = %ld\n", token_count);

    free(code);

    return 0;
}
