#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>

#define TOKEN_TYPE_NUM 33

struct token tokens[65536];

// 请开两个窗口将这个和lexer.h中enum token_type定义对比着读
// 每行对应相对的token类型匹配用的正则
const char *token_patterns[] = {
    "[ |\t]+",
    "[\n|\r\n]",
    "(/\\*([^\\*]|(\\*[^/]))*\\*/)|(//([^\r\n]|(\r[^\n]))*)", // C的正则不支持非贪婪，因此手动处理收尾
    "0|[1-9][0-9]*",
    "\"(\\.|[^\"])*\"",
    "int",
    "if",
    "else",
    "while",
    "do",
    "for",
    "break",
    "printf",
    ",",
    ";",
    "\\{",
    "\\}",
    "\\(",
    "\\)",
    "==",
    "!=",
    "<=",
    ">=",
    "<",
    ">",
    "=",
    "\\+\\+",
    "--",
    "\\+",
    "-",
    "\\*",
    "/",
    "([_a-zA-Z\u4e00-\u9fa5])([\u4e00-\u9fa5_a-zA-Z0-9])*",
    "."
};

// 用来存编译后的正则
regex_t token_re[TOKEN_TYPE_NUM];

void token_regex_init()
{
    for (size_t i = 0; i < TOKEN_TYPE_NUM; i++)
        regcomp(token_re + i, token_patterns[i], REG_EXTENDED);
    return;
}

struct token token_scan(char *code)
{
    static int line = 1;
    struct token token = { SP, NULL, 0, 0 };

    size_t max_match_len = 0;
    enum token_type max_match_type = (enum token_type)0;

    regmatch_t matched;

    // 寻找最长匹配，选择匹配到的最长的token
    for (size_t i = 0; i < TOKEN_TYPE_NUM; i++) {
        int match_status = regexec(token_re + i, code, 1, &matched, 0);
        if (matched.rm_so == 0 && match_status != REG_NOMATCH) {
            if (max_match_len < matched.rm_eo) {
                max_match_len = matched.rm_eo;
                max_match_type = (enum token_type)i;
            }
        }
    }

    regexec(token_re + (size_t)max_match_type, code, 1, &matched, 0);
    token.type = max_match_type;
    token.literal = code;
    token.length = (size_t)matched.rm_eo;

    // 处理行号，扫描token.literal，如果找到换行符则行号增加
    // '\r\n' 和 '\n' 中均有 '\n'，且仅在此处有，因此扫描 '\n'
    token.line = line;
    for (size_t i = 0; i < token.length; i++)
        if (token.literal[i] == '\n')
            line++;

    return token;
}

// 把注释 (非 SP, NL 或 COMMENT) 填进 tokens[]
// 返回 tokens[] 的长度（即所有token个数），最后一个元素用专属token类型END封住
size_t token_fill(char *iter)
{
    // C不能用非编译时常量初始化全局作用域的变量
    token_regex_init();

    size_t token_count = 0;
    while (*iter && *iter != EOF) {
        struct token t = token_scan(iter);
        iter += t.length;

        if (t.type == SP || t.type == NL || t.type == COMMENT)
            continue;
        tokens[token_count] = t;
        token_count++;
    }
    tokens[token_count].type = END;

    return token_count;
}
