from collections import Counter

from apt.auth import add_key

import common as c
import unittest


def _target(name: str) -> bool: return name.startswith("t")


def _answer1(lines: []) -> int:
    counter = Counter()
    links = {}

    def add_link(from_c, to_c: str):
        if from_c in links:
            links[from_c].add(to_c)
        else:
            links[from_c] = {to_c}

    computers = set({})
    for l in lines:
        cl = l.split("-")
        computers.update(cl)
        counter.update(cl)
        a, b = cl
        add_link(a, b)
        add_link(b, a)

    first_computer = [name for name in computers if _target(name) and counter[name] >= 2]
    result = 0
    for a in first_computer:
        candidates = list(set([l for l in links[a] if counter[l] >= 2]))
        for i, b in enumerate(candidates):
            for j, c in enumerate(candidates):
                if i < j:
                    result += 1
    return result


def _answer2(lines: []) -> int:
    return 0


class Day23(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        self.assertEqual(a1, _answer1(lines), "answer 1")
        self.assertEqual(a2, _answer2(lines), "answer 2")

    def test_sample(self):
        self.__solution("23-1", 0, 0)

    def test_day(self):
        self.__solution("23", 0, 0)
