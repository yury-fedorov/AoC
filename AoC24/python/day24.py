from typing import NamedTuple

import common as c
import unittest


class Formula(NamedTuple):
    a: str
    b: str
    op: str
    r: str


def _bin2int(value: str) -> int: return int(value, 2)


def _z(registers: {str: bool}) -> int:
    n = sum(1 for ri in registers.keys() if ri.startswith("z"))
    result = ""
    for i in range(n):
        result = ("1" if registers[f"z{i:02d}"] else "0") + result
    return _bin2int(result)


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(lines: []) -> int:
    return 0


def _answer2(lines: []) -> int:
    return 0


class Day24(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        self.assertEqual(a1, _answer1(lines), "answer 1")
        self.assertEqual(a2, _answer2(lines), "answer 2")

    def test_z(self):
        self.assertEqual(4, _z({"z00": 0, "z01": 0, "z02": 1}), "z case 1")
        self.assertEqual(2024, _bin2int("0011111101000"), "_bin2int")

    def test_sample1(self):
        self.__solution("24-1", 0, 0)

    def test_sample2(self):
        self.__solution("24-2", 0, 0)

    def test_day(self):
        self.__solution("24", 0, 0)
