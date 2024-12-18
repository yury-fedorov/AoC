from collections import Counter

import common as c
import unittest
from typing import NamedTuple
from common import Point
import re
import math


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

    def sign(x: float) -> int:
        if x == 0.0:
            return 0
        return 1 if x > 0 else -1

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
    for seconds in range(1, 10000):
        robots1 = []
        robo_set = set()
        for r in robots:
            p = _move(r, _SPACE, 1)
            robots1.append(Robot(p, r.velocity))
            robo_set.add(p)
        if len(robo_set) == len(robots):
            return seconds
        robots = robots1


class Day14(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        robots = [_parse(l) for l in lines]
        self.assertEqual(a1, _answer1(robots), "answer 1")
        self.assertEqual(a2, _answer2(robots), "answer 2")

    def test_moves(self):
        velocity = Point(2, -3)
        r = Robot(Point(2, 4), velocity)
        p1 = _move(r, _SPACE_SAMPLE, 1)
        self.assertEqual(Point(4, 1), p1, "move 1")
        p2 = _move(Robot(p1, velocity), _SPACE_SAMPLE, 1)
        self.assertEqual(Point(6, 5), p2, "move 2")
        p3 = _move(Robot(p2, velocity), _SPACE_SAMPLE, 1)
        self.assertEqual(Point(8, 2), p3, "move 3")
        p4 = _move(Robot(p3, velocity), _SPACE_SAMPLE, 1)
        self.assertEqual(Point(10, 6), p4, "move 4")
        p5 = _move(Robot(p4, velocity), _SPACE_SAMPLE, 1)
        self.assertEqual(Point(1, 3), p5, "move 5")
        p5a = _move(r, _SPACE_SAMPLE, 5)
        self.assertEqual(p5, p5a, "move 5 alternatives")

    def test_sample(self):
        lines = c.read_lines("14-1")
        robots = [_parse(l) for l in lines]
        self.assertEqual(12, _answer_space(robots, _SPACE_SAMPLE), "answer 1")

    def test_day(self):
        self.__solution("14", 221655456, 7858)  # takes 6.783s
