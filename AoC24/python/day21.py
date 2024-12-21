import common as c
import unittest


def _shortest_sequence(code: str) -> str:
    return code


def _complexity(code: str, length: int) -> int:
    return int(code[:-1]) * length


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(lines: [str]) -> int:
    return 0


def _answer2(lines: [str]) -> int:
    return 0


class Day21(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        codes = c.read_lines(data)
        self.assertEqual(a1, _answer1(codes), "answer 1")
        self.assertEqual(a2, _answer2(codes), "answer 2")

    def test_complexity(self):
        seq = "<vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A"
        self.assertEqual(68 * 29, _complexity("029A", len(seq)), "answer 1")

    def test_sample(self):
        self.__solution("21-1", 0, 0)

    def test_day(self):
        self.__solution("21", 0, 0)
