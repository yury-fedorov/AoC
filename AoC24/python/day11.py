import common as c
import unittest
from collections import Counter


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


def _answer1(stones: []) -> int:
    return _a2(stones, 25)


def _answer2(stones: []) -> int:
    return _a2(stones, 75)


# (value,times) - amount of stones
CACHE = {}


def _a2(stones: [], times: int) -> int:
    if times <= 0:
        return len(stones)
    if times == 1:
        return len(_blink(stones, times))

    if len(stones) == 1:
        # single stone case
        s = stones[0]
        # the cache key is stone and times
        key = (s, times)
        try:
            result = CACHE[key]
        except KeyError:
            result = _a2(_blink(stones, 1), times - 1)
            CACHE[key] = result
        return result

    # splitting on stones one by one
    counter = Counter(stones)
    result = 0
    for s_i in counter.keys():
        s_i_n = counter[s_i]
        result += s_i_n * _a2([s_i], times)
    return result


class Day11(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        stones = [int(x) for x in c.read_lines(data)[0].split()]
        self.assertEqual(a1, _answer1(stones), "answer 1")
        self.assertEqual(a2, _answer2(stones), "answer 2")

    def test_answer1(self):
        self.assertEqual(55312, _answer1([125, 17]), "answer 1")

    def test_blink(self):
        self.assertEqual(7, len(_blink([0, 1, 10, 99, 999], 1)), "blink")

    def test_day(self):
        self.__solution("11", 207683, 244782991106220)
