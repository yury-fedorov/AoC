import common as c
import unittest

def _is_possible(patterns: [str], to_make: str, made: str) -> bool:
    if len(to_make) == 0 or to_make in patterns:
        return True
    appliable = [p for p in patterns if to_make.find(p) == 0]
    for a in appliable:
        to_make1 = to_make[len(a):]
        made1 = made + a
        ok = _is_possible(patterns, to_make1, made1)
        if ok:
            return True
    return False

# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(patterns: [str], designs: [str]) -> int:
    return sum(1 for d in designs if _is_possible(patterns, d, ""))


def _answer2(patterns: [str], designs: [str]) -> int:
    return 0


class Day19(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        patterns = lines[0].split(", ")
        designs = lines[2:]
        self.assertEqual(a1, _answer1(patterns, designs), "answer 1")
        self.assertEqual(a2, _answer2(patterns, designs), "answer 2")

    def test_sample(self):
        self.__solution("19-1", 6, 0)

    def test_day(self):
        self.__solution("19", 0, 0)
