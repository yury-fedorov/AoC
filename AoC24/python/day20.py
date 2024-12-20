from typing import Sequence

import common as c
import unittest

START = 'S'
END = 'E'
WALL = '#'
TRACK = '.'

MIN_SAFE = 100  # for answer 1

SHIFTS = [c.Point(1, 0), c.Point(0, 1), c.Point(-1, 0), c.Point(0, -1)]


def _find_location(the_map: [str], what: str) -> c.Point:
    for y, line in enumerate(the_map):
        x = line.find(what)
        if x >= 0: return c.Point(x, y)
    raise ValueError()


def _at(the_map: [str], x, y: int) -> str:
    if x < 0 or y < 0 or y >= len(the_map) or x >= len(the_map[0]): return WALL
    return the_map[y][x]


def _walls(the_map: [str]) -> [c.Point]:
    return [c.Point(x, y) for y in range(1, len(the_map))
            for x in range(1, len(the_map[0]))
            if _at(the_map, x, y) == WALL]


def _shortest_distance(the_map: [str], start, end: c.Point, max_distance: int | None = None) -> int | None:
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
                p1 = c.Point(x1, y1)
                if p1 == end: return distance + 1  # wall can be destination
                what = _at(the_map, x1, y1)
                if what != WALL and (not p1 in visited):
                    next_frontline.add(p1)
        frontline = next_frontline
        distance += 1
        if max_distance is not None and distance > max_distance: return None

    return None


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(the_map: [str], max_desired_shortest: int) -> int:
    start = _find_location(the_map, START)
    end = _find_location(the_map, END)
    walls = _walls(the_map)
    count = 0
    for i, w in enumerate(walls):
        start_to_wall = _shortest_distance(the_map, start, w, max_desired_shortest)
        if start_to_wall is None or start_to_wall >= max_desired_shortest: continue
        wall_to_end = _shortest_distance(the_map, w, end, max_desired_shortest - start_to_wall)
        if wall_to_end is not None and start_to_wall + wall_to_end <= max_desired_shortest:
            count += 1
    return count


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
        the_map = c.read_lines("20-1")
        start = _find_location(the_map, START)
        end = _find_location(the_map, END)
        shortest = _shortest_distance(the_map, start, end)
        max_desired_shortest = shortest - 20
        self.assertEqual(5, _answer1(the_map, max_desired_shortest), "answer 1")

    def test_day(self):
        the_map = c.read_lines("20")
        start = _find_location(the_map, START)
        end = _find_location(the_map, END)
        shortest = _shortest_distance(the_map, start, end)
        max_desired_shortest = shortest - MIN_SAFE
        self.assertEqual(1415, _answer1(the_map, max_desired_shortest), "answer 1")  # takes 39 minutes
