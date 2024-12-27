import common as c
import unittest


def _read_input(data: str):
    left = []
    right = []
    with c.open_file(data) as f:
        for line in f:
            l, r = line.split()
            left.append(int(l))
            right.append(int(r))
    left.sort()
    right.sort()
    return left, right


def _answer1(left, right) -> int:
    return sum(abs(right[i] - left[i]) for i in range(0, len(left)))


def _answer2(left, right) -> int:  # similarity score
    return sum(left[i] * right.count(left[i]) for i in range(0, len(left)))


class Day01(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        left, right = _read_input(data)
        self.assertEqual(_answer1(left, right), a1, "answer 1")
        self.assertEqual(_answer2(left, right), a2, "answer 2")

    def test_sample(self):
        self.__solution("01-1", 11, 31)

    def test_day(self):
        self.__solution("01", 2344935, 27647262)
