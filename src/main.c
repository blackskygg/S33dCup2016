#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "syntax.h"
#include "evaluate.h"

FILE *fout = NULL;

extern struct token tokens[];
extern struct scope_record *scope;

char *read_code(const char *fname)
{
    FILE *fcode = fopen(fname, "rb");

    if (fcode == NULL) {
        perror("cannot open code file");
        exit(1);
    }

    fseek(fcode, 0, SEEK_END);
    size_t fsize = ftell(fcode);
    rewind(fcode);

    char *buf = (char *)malloc(sizeof(char) * (fsize + 1));
    fread(buf, 1, fsize, fcode);
    buf[fsize] = '\0';

    fclose(fcode);

    return buf;
}

int main(int argc, char const *argv[])
{
    token_regex_init();

    // MALLOC code
    char *code = read_code("input.txt");

    // STATIC tokens
    size_t token_count = token_fill(code);

    // DEBUG display
    for (size_t i = 0; i < token_count; i++) {
        printf("token: %d\t\"", tokens[i].type);
        for (size_t j = 0; j < tokens[i].length; j++)
            putchar(tokens[i].literal[j]);
        printf("\"\t%ld\n", tokens[i].line);
    }
    printf("token_count = %ld\n", token_count);

    // MALLOC root
    struct syntax_node *root = generate_ast();

    fout = fopen("output.txt", "w");

    // DEBUG display
    print_ast(root, 0);

    enter_scope();
    eval_stat_list(root);
    leave_scope();

    fclose(fout);

    // FREE root
    destory_ast(root);

    // FREE code
    free(code);

    return 0;
}
