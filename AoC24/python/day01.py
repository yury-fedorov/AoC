import common as c
import unittest


def _read_input(data: str):
    left = []
    right = []
    with c.open_file(data) as f:
        for line in f:
            x = line.split()
            left.append(int(x[0]))
            right.append(int(x[1]))
    left.sort()
    right.sort()
    return left, right


def _answer1(left, right) -> int:
    diff = 0
    for i in range(0, len(left)):
        diff += abs(right[i] - left[i])
    return diff


def _answer2(left, right) -> int:
    score = 0  # similarity score
    for i in range(0, len(left)):
        score += left[i] * right.count(left[i])
    return score


class Day01(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        left, right = _read_input(data)
        self.assertEqual(_answer1(left, right), a1, "answer 1")
        self.assertEqual(_answer2(left, right), a2, "answer 2")

    def test_sample(self):
        self.__solution("01-1", 11, 31)

    def test_day(self):
        self.__solution("01", 2344935, 27647262)
