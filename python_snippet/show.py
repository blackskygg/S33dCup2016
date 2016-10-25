# coding=utf-8

with open("input.txt") as raw_code:
    lines = raw_code.readlines()

with open("output.txt") as raw_seq:
    seq = map(lambda x: int(x) - 1, raw_seq.read().split())

for curr_line in seq:
    print("-" * 80)
    for i, line in enumerate(lines):
        if i == curr_line:
            print("\033[31m" + line + "\033[0m", end="")
        else:
            print(line, end="")
    input()
