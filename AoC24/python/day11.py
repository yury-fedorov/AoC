import common as c
import unittest
from collections import Counter
import time


def _blink(stones: [], times: int) -> []:
    if times <= 0:
        return stones

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

    return _blink(new_stones, times - 1)


def _brute_force(stones: [], times: int) -> int:
    return len(_blink(stones, times))


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(stones: []) -> int:
    return _brute_force(stones, 25)


MAX_LENGTH = 40 # perfect length for performance (4 times faster on 25 times)
MAX_DEPTH = 5
CACHE = {}


def _a2(stones: [], times: int) -> int:
    n = len(stones)
    if times <= 0:
        return n

    if n == 1 and times >= MAX_DEPTH:
        # single element we cache
        s = stones[0]
        try:
            next_five = CACHE[s]
        except KeyError:
            next_five = _blink(stones, MAX_DEPTH)
            CACHE[s] = next_five
        return _a2(next_five, times - MAX_DEPTH)

    elif n < MAX_LENGTH:
        return _a2(_blink(stones, 1), times - 1)

    # we need to split
    counter = Counter(stones)
    result = 0
    for s_i in counter.keys():
        s_i_n = counter[s_i]
        result += s_i_n * _a2([s_i], times)
    return result


def _answer2(stones: []) -> int:
    return _a2(stones, 75)


class Day11(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        stones = [int(x) for x in c.read_lines(data)[0].split()]
        self.assertEqual(a1, _answer1(stones), "answer 1")
        # if not c.is_fast_only():
        self.assertEqual(a2, _answer2(stones), "answer 2")

    def test_sample1(self):
        self.assertEqual(55312, _answer1([125, 17]), "answer 1")

    def test_sample(self):
        self.assertEqual(7, _brute_force([0, 1, 10, 99, 999], 1), "answer 1")

    def test_sample2(self):
        s = [125, 17]
        ts0 = time.time()
        a1 = _answer1(s)
        ts1 = time.time()
        a2 = _a2(s, 25)
        ts2 = time.time()
        dt1 = ts1 - ts0
        dt2 = ts2 - ts1
        self.assertEqual( a1, a2, "answer 1 vs answer 2")

    def test_day(self):
        self.__solution("11", 207683, 0)
