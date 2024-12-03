import common as c
import math
import unittest

sign = lambda x: math.copysign(1, x)


def _read_levels(data: str):
    levels = []
    with c.open_file(data) as f:
        for line in f:
            levels.append([int(lv) for lv in line.split()])
    return levels


def _calc_diff(level):
    return [level[i] - level[i - 1] for i in range(1,len(level))]


def _is_safe(level) -> bool:
    diff = _calc_diff(level)
    direction = None
    for d in diff:
        if not (1 <= abs(d) <= 3):
            return False
        direction_x = sign(d)
        if (direction is not None) and (direction_x != direction):
            return False
        direction = direction_x

    return True


def _is_safe_2(level) -> bool:
    if _is_safe(level):
        return True
    n = len(level)
    for i in range(0, n):
        m_level = level[:]
        m_level.pop(i)
        if _is_safe(m_level):
            return True

    return False


def _answer(levels, is_safe_f) -> int:
    return sum(1 if is_safe_f(level) else 0 for level in levels)


class Day02(unittest.TestCase):

    @staticmethod
    def __solution(data: str, a1: int, a2: int):
        answer1 = lambda levels: _answer(levels, _is_safe)
        answer2 = lambda levels: _answer(levels, _is_safe_2)
        levels = _read_levels(data)
        assert answer1(levels) == a1, "answer 1"
        assert answer2(levels) == a2, "answer 2"

    def test_sample(self):
        self.__solution("02-1", 2, 4)

    def test_day(self):
        self.__solution("02", 549, 589)
