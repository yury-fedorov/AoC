import common as c
import unittest


def _answer1(the_map: []) -> int:
    return 0


def _answer2(the_map: []) -> int:
    return 0


class Day08(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = c.read_lines(data)
        self.assertEqual(_answer1(the_map), a1, "answer 1")
        self.assertEqual(_answer2(the_map), a2, "answer 2")

    def test_sample(self):
        self.__solution("08-1", 14, 0)

    def test_day(self):
        self.__solution("08", 0, 0)
