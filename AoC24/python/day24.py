import common as c
import unittest


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(lines: []) -> int:
    return 0


def _answer2(lines: []) -> int:
    return 0


class Day24(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        self.assertEqual(_answer1(lines), a1, "answer 1")
        self.assertEqual(_answer2(lines), a2, "answer 2")

    def test_day(self):
        self.__solution("01", 0, 0)
