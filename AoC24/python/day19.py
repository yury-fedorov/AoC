import datetime
import re

import common as c
import unittest


def _create_map(patterns: [str]) -> {str: [str]}:
    result = {}
    letters = set([p[0] for p in patterns])
    for l in letters:
        sub = [p for p in patterns if p.startswith(l)]
        sub.sort(key=len, reverse=True)
        result[l] = sub
    return result


def _is_possible(patterns: [str], pat_map: {str: [str]}, to_make: str) -> bool:
    if len(to_make) == 0 or to_make in patterns:
        return True
    appliable_start = [p for p in pat_map.get(to_make[0], []) if to_make.startswith(p)]
    for a in appliable_start:
        to_make1 = to_make[len(a):]
        # appliable_end = [p for p in patterns if to_make1.endswith(p)]
        # for b in appliable_end:
        #    to_make1 = to_make[:-len(b)]
        if _is_possible(patterns, pat_map, to_make1):
            return True
    return False


def _create_re(patterns: [str]):
    return re.compile(f"({'|'.join(patterns)})+")


def _is_possible(patterns: [str], pattern, to_make: str) -> bool:
    n = len(to_make)
    if n == 0 or to_make in patterns: return True
    if n > 15:
        hn = int(n / 2) - 2  # average length of a pattern 4
        for p in patterns:
            index = to_make.find(p, hn)
            if index >= 0:
                head = to_make[:index]
                tail = to_make[index + len(p):]
                ok = _is_possible(patterns, pattern, head)
                if ok:
                    ok = _is_possible(patterns, pattern, tail)
                    if ok:
                        return True
        return False
    # print("Checking " + to_make)
    return pattern.fullmatch(to_make) is not None


def _count_possible2(patterns: [str], to_make: str) -> int:
    n = len(to_make)
    if n == 0: return 1
    count = 0
    if n > 15:
        hn = int(n / 2) - 2  # average length of a pattern 4
        for p in patterns:
            index = to_make.find(p, hn)
            if index >= 0:
                head = to_make[:index]
                tail = to_make[index + len(p):]
                ch = _count_possible2(patterns, head)
                if ch > 0:
                    ct = _count_possible2(patterns, tail)
                    if ct > 0:
                        count += ch * ct
        return count
    for p in patterns:
        if to_make.startswith(p):
            tail = to_make[len(p):]
            ct = _count_possible2(patterns, tail)
            count += ct
    return count


def _answer1(patterns: [str], designs: [str]) -> int:
    regexp = _create_re(patterns)
    return sum(1 for d in designs if _is_possible(patterns, regexp, d))


def _answer2(patterns: [str], designs: [str]) -> int:
    count = 0
    regexp = _create_re(patterns)
    for i, d in enumerate(designs):
        print(f"{datetime.datetime.now()} Design {i} : {d}")
        # if  _is_possible(patterns, pat_map, d):
        if _is_possible(patterns, regexp, d):
            ci = _count_possible2(patterns, d)
            count += ci
            print(f"... counted {ci}")
    return count


class Day19(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        patterns = lines[0].split(", ")
        designs = lines[2:]
        self.assertEqual(a1, _answer1(patterns, designs), "answer 1")
        self.assertEqual(a2, _answer2(patterns, designs), "answer 2")

    def test_sample(self):
        self.__solution("19-1", 6, 16)

    # 20 seconds for answer1
    def test_day(self):
        self.__solution("19", 298, 0)
