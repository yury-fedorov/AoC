import common as c
import re
import unittest


def read_input(data: str):
    lines = []
    with c.open_file(data) as f:
        for line in f:
            lines.append(line)
    return lines

def find_all_matches(string):
    pat = re.compile(r'mul(.)(\d+),(\d+)(.)')
    pos = 0
    out = []
    while m := pat.search(string, pos):
        pos = m.start() + 1
        if m[1] == '(' and m[4] == ')':
            out.append(int(m[2]) * int(m[3]))
    return out

def answer1(lines) -> int:
    result = 0

    for line in lines:
        for m in find_all_matches(line):
            result += m
    return result


def answer2(lines) -> int:
    return 0


class Day03(unittest.TestCase):

    def x_test(self, data: str, a1: int, a2: int):
        lines = read_input(data)
        self.assertEqual(answer1(lines), a1, "answer 1")
        # self.assertEqual(answer2(lines), a2, "answer 2")

    def test_sample(self):
        self.x_test("03-1", 161, 0)

    def test_day(self):
        self.x_test("03", 161289189, 0)
