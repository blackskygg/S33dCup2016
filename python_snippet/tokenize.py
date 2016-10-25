# coding=utf-8

import re

keywords = [
    "int",
    "printf",
    "if",
    "else",
    "for",
    "while",
    "do"
]

type_name = [
    " ",
    "\n",
    "SYM",
    "INT",
    "STR",
    "COM1",
    "COM2",
    ";",
    ",",
    "=",
    "(",
    ")",
    "{",
    "}",
    "<",
    ">",
    "==",
    "!=",
    "<=",
    ">=",
    "+",
    "-",
    "*",
    "/",
    "++",
    "--"
]

patterns = [
    re.compile(pattern) for pattern in
    [r'(?: |\t)+',
        r'\n',  # python automatically translate \r\n into \n
        r'[_a-zA-Z][_a-zA-Z0-9]*',
        r'(?:0|[1-9][0-9]*)',
        r'"(?:\\.|[^"])*"',
        r'\/\*(?:.|\n)*?\*\/',  # CRLF -> LF, same as above
        r'\/\/.*\n',  # CRLF -> LF, same as above
        r';',
        r',',
        r'=',
        r'\(',
        r'\)',
        r'{',
        r'}',
        r'<',
        r'>',
        r'=',
        r'!=',
        r'<=',
        r'>=',
        r'\+',
        r'-',
        r'\*',
        r'\/',
        r'\+\+',
        r'--'
     ]
]

fin = open("input.txt")
raw = fin.read()

tokens = []
while raw:
    candidates = [
        m.end() if m else 0
        for m in [pattern.match(raw) for pattern in patterns]
    ]

    type_i = 0
    token_len = 0
    for i in range(len(candidates)):
        if candidates[i] > token_len:
            token_len = candidates[i]
            type_i = i
    token, raw = raw[:token_len], raw[token_len:]

    if type_name[type_i] == "COM1":
        for i in range(token.count('\n')):
            tokens.append("NL")
    elif type_name[type_i] == "COM2":
        tokens.append("NL")
    elif type_name[type_i] == "SYM":
        if token in keywords:
            tokens.append(token)
        else:
            tokens.append((type_name[type_i], token))
    elif type_name[type_i] == "INT":
        tokens.append((type_name[type_i], token))
    else:
        tokens.append(type_name[type_i])

for t in tokens:
    print(repr(t))
