import common as c
import unittest


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(stones: []) -> int:
    for _ in range(25):
        new_stones = []
        for s in stones:
            if s == 0:
                new_stones.append(1)
            else:
                ss = str(s)
                n = len(ss)
                if n % 2 == 0:
                    hn = int(n / 2)
                    sl = int(ss[0:hn])
                    sr = int(ss[hn:])
                    new_stones.extend([sl, sr])
                else:
                    new_stones.append(s * 2024)
        stones = new_stones
    return len(stones)


def _answer2(stones: []) -> int:
    return 0


class Day11(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        stones = [int(x) for x in c.read_lines(data)[0].split()]
        self.assertEqual(a1, _answer1(stones), "answer 1")
        self.assertEqual(a2, _answer2(stones), "answer 2")

    def test_sample1(self):
        self.assertEqual(55312, _answer1([125, 17]), "answer 1")

    def test_sample(self):
        self.__solution("11-1", 0, 0)

    def test_day(self):
        self.__solution("11", 207683, 0)
