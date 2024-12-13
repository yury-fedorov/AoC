import common as c
import unittest
import numpy as np
import re
from typing import NamedTuple


class Equation(NamedTuple):
    ak: int
    bk: int
    r: int


class Solution(NamedTuple):
    a: int
    b: int


class System(NamedTuple):
    e1: Equation
    e2: Equation


def _solve1(e1: Equation, e2: Equation) -> Solution:
    k = np.array([[e1.ak, e1.bk], [e2.ak, e2.bk]])
    r = np.array([e1.r, e2.r])
    ab = np.linalg.solve(k, r)
    return Solution(round(ab[0]), round(ab[1]))


def _is_valid_solution(e1: Equation, e2: Equation, s: Solution) -> bool:
    def is_valid(e: Equation) -> bool:
        return (e.ak * s.a) + (e.bk * s.b) == e.r

    return is_valid(e1) and is_valid(e2)


def _parse_system(lines: [str]) -> System:
    # Button A: X+94, Y+34
    # Button B: X+22, Y+67
    # Prize: X=8400, Y=5400
    p1 = 'Button .: X(.\d+), Y(.\d+)'
    a = re.search(p1, lines[0])
    b = re.search(p1, lines[1])
    p2 = 'Prize: X=(\d+), Y=(\d+)'
    prize = re.search(p2, lines[2])
    e1 = Equation(int(a[1]), int(b[1]), int(prize[1]))
    e2 = Equation(int(a[2]), int(b[2]), int(prize[2]))
    return System(e1, e2)


def _parse(lines: [str]) -> [System]:
    n = int((len(lines) + 1) / 4)
    return [_parse_system(lines[(i * 4):]) for i in range(n)]


def _answer(systems: [System], delta_r: int) -> int:
    result = 0
    for s in systems:
        e1 = Equation(s.e1.ak, s.e1.bk, delta_r + s.e1.r)
        e2 = Equation(s.e2.ak, s.e2.bk, delta_r + s.e2.r)
        sol = _solve1(e1, e2)
        if _is_valid_solution(e1, e2, sol):
            result += (3 * sol[0]) + (sol[1])
    return result


def _answer1(systems: [System]) -> int:
    return _answer(systems, 0)


def _answer2(systems: [System]) -> int:
    return _answer(systems, 10000000000000)


class Day13(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        systems = _parse(lines)
        self.assertEqual(a1, _answer1(systems), "answer 1")
        self.assertEqual(a2, _answer2(systems), "answer 2")

    def test_valid_system(self):
        self.assertEqual((80, 40), _solve1(Equation(94, 22, 8400), Equation(34, 67, 5400)), "equation system")

    def test_invalid_system(self):
        e1 = Equation(26, 67, 12748)
        e2 = Equation(66, 21, 12176)
        s = _solve1(e1, e2)
        self.assertFalse(_is_valid_solution(e1, e2, s), "check solution")

    def test_sample(self):
        self.__solution("13-1", 480, 875318608908)

    def test_day(self):
        self.__solution("13", 33427, 91649162972270)
