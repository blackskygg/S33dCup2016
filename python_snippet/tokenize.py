# coding=utf-8

import re

type_name = [
    "SPACE",
    "CRLF",
    "SYMBOL",
    "INTEGER",
    "STRING",
    "COMMENT",
    "SEMICOLON",
    "COMMA",
    "ASSIGN",
    "LPARENTHESE",
    "RPARENTHESE",
    "LBRACE",
    "RBRACE",
    "LT",
    "GT",
    "EQ",
    "NE",
    "LE",
    "GE",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "INC",
    "DEC"
]

patterns = [
    re.compile(pattern) for pattern in
    [r'(?: |\t)+',
        r'\n',
        r'[_a-zA-Z][_a-zA-Z0-9]*',
        r'(?:0|[1-9][0-9]*)',
        r'"(?:\\.|[^"])*"',
        r'(?:\/\*.*?\*\/)|(?:\/\/.*)',
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
    candidates = map(lambda m: m.end() if m else 0,  [pattern.match(raw) for pattern in patterns])
    candidate_len = max(candidates)
    tokens.append(raw[:candidate_len])
    raw = raw[candidate_len:]

print("".join(tokens))
