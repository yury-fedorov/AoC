import common as c
import unittest


def _count_possible(patterns: [str], to_make: str, cache: {str: int}) -> int:
    if not any(to_make): return 1
    cached = cache.get(to_make, None)
    if cached is not None: return cached
    count = sum(_count_possible(patterns, to_make[len(p):], cache) for p in patterns if to_make.startswith(p))
    cache[to_make] = count
    return count


def _answer1(patterns: [str], designs: [str]) -> int:
    cache = {}
    return sum(1 for d in designs if _count_possible(patterns, d, cache) > 0)


def _answer2(patterns: [str], designs: [str]) -> int:
    cache = {}
    return sum(_count_possible(patterns, d, cache) for d in designs)


class Day19(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        patterns = lines[0].split(", ")
        designs = lines[2:]
        self.assertEqual(a1, _answer1(patterns, designs), "answer 1")
        self.assertEqual(a2, _answer2(patterns, designs), "answer 2")

    def test_sample(self):
        self.__solution("19-1", 6, 16)

    def test_day(self):
        self.__solution("19", 298, 572248688842069)
