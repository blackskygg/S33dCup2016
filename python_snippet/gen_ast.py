# coding=utf-8

# cannot parse comments

import pycparser

with open("input.txt") as f:
    raw = "void program() {\n" + f.read() + "}\n"

parser = pycparser.c_parser.CParser()
ast = parser.parse(raw, filename='input.txt').ext[0].body

ast.show()
