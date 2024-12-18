import common as c
import unittest
from common import Point

START = Point(0, 0)
END = Point(70, 70)
SAMPLE_END = Point(6, 6)
SHIFTS = [Point(1, 0), Point(0, 1), Point(-1, 0), Point(0, -1)]
KB = 1024


class PointInTime(Point):
    since: int


def _create_static_map(lines: [Point], till: int) -> {Point}:
    the_map = set({})
    for p in lines[:till]:
        the_map.add(p)
    return the_map


def _shortest_distance(the_map: {Point}, start: Point, end: Point) -> int:
    visited = set({})
    frontline = [start]
    distance = 0
    while True:
        next_frontline = set({})
        for cur_point in frontline:
            if cur_point == end:
                return distance
            for s in SHIFTS:
                x1 = cur_point.x + s.x
                y1 = cur_point.y + s.y
                if x1 < 0 or y1 < 0 or x1 > end.x or y1 > end.y or cur_point in the_map or cur_point in visited:
                    continue
                next_frontline.add(Point(x1, y1))
            visited.add(cur_point)
        distance += 1
        frontline = next_frontline


def _print_map(the_map: {Point}):
    for y in range(1 + max([p.y for p in the_map])):
        line = ""
        for x in range(1 + max([p.x for p in the_map])):
            line += "#" if Point(x, y) in the_map else "."
        print(line)


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(lines: [Point], till: int, end: Point) -> int:
    the_map = _create_static_map(lines, till)
    # _print_map(the_map)
    return _shortest_distance(the_map, START, end)


def _answer2(lines: [Point]) -> int:
    return 0


def _parse(lines: [str]) -> [Point]:
    def to_point(line: str) -> Point:
        x, y = line.split(",")
        return Point(int(x), int(y))

    return list(map(to_point, lines))


class Day18(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int, till: int, end: Point):
        the_list = _parse(c.read_lines(data))
        self.assertEqual(a1, _answer1(the_list, till, end), "answer 1")
        self.assertEqual(a2, _answer2(the_list), "answer 2")

    def test_sample(self):
        self.__solution("18-1", 22, 0, 12, SAMPLE_END)

    def test_day(self):
        self.__solution("18", 246, 0, KB, END)
