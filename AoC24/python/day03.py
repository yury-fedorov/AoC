import common as c
import re
import unittest


def _read_input(data: str) -> str:
    result = ''
    with c.open_file(data) as f:
        for line in f:
            result += line
    return result


def _find_all_matches(string):
    pat = re.compile(r'mul\((\d+),(\d+)\)')
    pos = 0
    out = []
    while m := pat.search(string, pos):
        pos = m.start() + 1
        out.append(int(m[1]) * int(m[2]))
    return out


def _answer1(line) -> int:
    result = 0
    for m in _find_all_matches(line):
        result += m
    return result


def _answer2(line) -> int:
    result = 0

    while len(line) > 0:
        line_ok: str
        index_end = line.find("don't()")
        if index_end >= 0:
            line_ok = line[:index_end]
            index_start = line.find("do()", index_end)
            line = line[index_start:] if index_start >= 0 else ""
        else:
            line_ok = line
            line = ""
        for m in _find_all_matches(line_ok):
            result += m

    return result


class Day03(unittest.TestCase):

    def _test(self, data: str, a1: int, a2: int):
        code = _read_input(data)
        self.assertEqual(_answer1(code), a1, "answer 1")
        self.assertEqual(_answer2(code), a2, "answer 2")

    def test_sample(self):
        self._test("03-1", 161, 161)

    def test_sample2(self):
        self._test("03-2", 161, 48)

    def test_day(self):
        self._test("03", 161289189, 83595109)
