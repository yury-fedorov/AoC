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


def _shortest_distance(the_map: {Point}, start: Point, end: Point) -> int | None:
    visited = set({})
    frontline = [start]
    distance = 0
    while len(frontline):
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
    return None


def _answer1(lines: [Point], till: int, end: Point) -> int:
    the_map = _create_static_map(lines, till)
    return _shortest_distance(the_map, START, end)


def _answer21(lines: [Point], t_min: int, t_max: int, end: Point) -> str:
    def shortest(t: int) -> int | None:
        the_map = _create_static_map(lines, t)
        return _shortest_distance(the_map, START, end)

    t_mid = int((t_max + t_min) / 2)
    if t_min < t_mid < t_max:
        d_mid = shortest(t_mid)
        if d_mid is None:
            return _answer21(lines, t_min, t_mid, end)
        else:
            return _answer21(lines, t_mid, t_max, end)
    p = lines[t_min]
    return f"{p.x},{p.y}"


def _answer2(lines: [Point], end: Point) -> str: return _answer21(lines, 0, len(lines) - 1, end)


def _parse(lines: [str]) -> [Point]:
    def to_point(line: str) -> Point:
        x, y = line.split(",")
        return Point(int(x), int(y))

    return list(map(to_point, lines))


class Day18(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int, till: int, end: Point):
        the_list = _parse(c.read_lines(data))
        self.assertEqual(a1, _answer1(the_list, till, end), "answer 1")
        self.assertEqual(a2, _answer2(the_list, end), "answer 2")

    def test_sample(self):
        self.__solution("18-1", 22, "6,1", 12, SAMPLE_END)

    def test_day(self):
        self.__solution("18", 246, "22,50", KB, END)
