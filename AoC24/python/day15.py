import common as c
import unittest


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(the_map: [str]) -> int:
    return 0


def _answer2(the_map: [str]) -> int:
    return 0


class Day15(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = c.read_lines(data)
        self.assertEqual(a1, _answer1(the_map), "answer 1")
        self.assertEqual(a2, _answer2(the_map), "answer 2")

    def test_sample(self):
        self.__solution("15-2", 0, 0)

    def test_day(self):
        self.__solution("15", 0, 0)
