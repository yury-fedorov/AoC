from collections import Counter

import common as c
import unittest
from typing import NamedTuple
import re
import math


class Point(NamedTuple):
    x: int
    y: int


class Robot(NamedTuple):
    position: Point
    velocity: Point


_SECONDS = 100
_SPACE_SAMPLE = Point(11, 7)
_SPACE = Point(101, 103)


def _move(robot: Robot, space: Point, times: int) -> Point:
    x, y = robot.position
    dx, dy = robot.velocity
    sx, sy = space
    for _ in range(times):
        x += dx
        y += dy
        if x < 0:
            x += sx
        elif x >= sx:
            x -= sx
        if y < 0:
            y += sy
        elif y >= sy:
            y -= sy
    return Point(x, y)


_PATTERN = re.compile(r"p=(\d+),(\d+) v=(.?\d+),(.?\d+)")


def _parse(line: str) -> Robot:
    # p=0,4 v=3,-3
    m = re.search(_PATTERN, line)
    return Robot(Point(int(m[1]), int(m[2])), Point(int(m[3]), int(m[4])))


def _answer_space(robots: [Robot], space: Point) -> int:
    def move(r: Robot) -> Point:
        return _move(r, space, 100)

    final_positions = [move(r) for r in robots]
    sign = lambda x: int(math.copysign(1, x))

    def quadrant(p: Point) -> Point:
        x, y = p
        sx, sy = space
        sxh = int(sx / 2)
        syh = int(sy / 2)
        return Point(sign(x - sxh), sign(y - syh))

    ql = [quadrant(p) for p in final_positions]
    counter = Counter(ql)
    qc = [count for (x, y), count in counter.items() if x != 0 and y != 0]
    return math.prod(qc)


def _answer1(robots: [Robot]) -> int:
    return _answer_space(robots, _SPACE)


def _answer2(robots: []) -> int:
    return 0


class Day14(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        robots = [_parse(l) for l in lines]
        self.assertEqual(a1, _answer1(robots), "answer 1")
        self.assertEqual(a2, _answer2(robots), "answer 2")

    def test_sample(self):
        lines = c.read_lines("14-1")
        robots = [_parse(l) for l in lines]
        self.assertEqual(12, _answer_space(robots, _SPACE_SAMPLE), "answer 1")

    def test_day(self):
        self.__solution("14", 0, 0)
