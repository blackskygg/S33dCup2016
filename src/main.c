#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "evaluate.h"

int main(int argc, char const *argv[])
{
    char *code = NULL;
    const char *fname = NULL;

    if (argc > 1)
        fname = argv[1];
    else
        fname = "input.txt";

    FILE *fcode = fopen(fname, "rb");

    if (fcode == NULL) {
        perror("cannot open code file");
        exit(1);
    }

    // 获取文件长度
    fseek(fcode, 0, SEEK_END);
    size_t fsize = ftell(fcode);
    rewind(fcode);

    // MALLOC code
    code = (char *)malloc(sizeof(char) * (fsize + 1));

    // 读取文件并封0
    fread(code, 1, fsize, fcode);
    code[fsize] = '\0';

    fclose(fcode);

    // STATIC tokens
    size_t token_count = token_fill(code, fsize);

    // DEBUG display
    for (size_t i = 0; i < token_count; i++) {
        printf("token: %d\tlength: [%ld]\t\"", tokens[i].type, tokens[i].length);
        for (size_t j = 0; j < tokens[i].length; j++)
            putchar(tokens[i].literal[j]);
        printf("\"\t%ld\n", tokens[i].line);
    }
    printf("token_count = %ld\n", token_count);

    // MALLOC root
    struct syntax_node *root = generate_ast();

    // 这个变量声明在evaluate.h中，定义在evaluate.c
    // 用来传输出文件（显然）
    fout = fopen("output.txt", "w");

    // DEBUG display
    print_ast(root, 0);

    enter_scope();  // 全局就是一个作用域
    eval_stat_list(root);
    leave_scope();  // 离开全局作用域

    fclose(fout);

    // FREE root
    destory_ast(root);

    token_destroy();

    // FREE code
    free(code);

    return 0;
}
