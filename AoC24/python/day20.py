import common as c
import unittest

START = 'S'
END = 'E'
WALL = '#'
TRACK = '.'

SHIFTS = [c.Point(1, 0), c.Point(0, 1), c.Point(-1, 0), c.Point(0, -1)]


def _find_location(the_map: [str], what: str) -> c.Point:
    for y, line in enumerate(the_map):
        x = line.find(what)
        if x >= 0: return c.Point(x, y)
    raise ValueError()


def _at(the_map: [str], x, y: int) -> str:
    if x < 0 or y < 0 or y >= len(the_map) or x >= len(the_map[0]): return WALL
    return the_map[y][x]


def _shortest_distance(the_map: [str], start, end: c.Point) -> int | None:
    if start == end: return 0

    distance = 0
    visited = set({})
    frontline = {start}
    while len(frontline):
        next_frontline = set({})
        for pos in frontline:
            if pos == end: return distance
            visited.add(pos)
            for s in SHIFTS:
                x1 = pos.x + s.x
                y1 = pos.y + s.y
                what = _at(the_map, x1, y1)
                p1 = c.Point(x1, y1)
                if what != WALL and (not p1 in visited):
                    next_frontline.add(p1)
        frontline = next_frontline
        distance += 1

    return None


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(the_map: [str]) -> int:
    return 0


def _answer2(the_map: [str]) -> int:
    return 0


class Day20(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = c.read_lines(data)
        self.assertEqual(a1, _answer1(the_map), "answer 1")
        self.assertEqual(a2, _answer2(the_map), "answer 2")

    def test_shortest(self):
        the_map = c.read_lines("20-1")
        start = _find_location(the_map, START)
        end = _find_location(the_map, END)
        self.assertEqual(84, _shortest_distance(the_map, start, end), "shortest")

    def test_sample(self):
        self.__solution("20-1", 0, 0)

    def test_day(self):
        self.__solution("20", 0, 0)
