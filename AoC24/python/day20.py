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


def _distance_through_walls(jump_start, jump_end: c.Point) -> int:
    return abs(jump_start.x - jump_end.x) + abs(jump_start.y - jump_end.y)


def _jump_distance(the_map: [str], start, jump_start, jump_end, end: c.Point, max_desired_shortest: int) -> int | None:
    start_to_wall = _shortest_distance(the_map, start, jump_start, max_desired_shortest)
    if start_to_wall is None or start_to_wall >= max_desired_shortest: return None
    distance_through_walls = _distance_through_walls(jump_start, jump_end)
    remaining_max_distance = max_desired_shortest - start_to_wall - distance_through_walls
    wall_to_end = _shortest_distance(the_map, jump_end, end, remaining_max_distance)
    if wall_to_end is None: return None
    total_distance = start_to_wall + distance_through_walls + wall_to_end
    return total_distance if total_distance <= max_desired_shortest else None


def _all_paths(the_map: [str], start, end: c.Point, max_steps: int) -> {c.Point}:
    result = {start}
    frontline = {start}
    distance = 0
    while len(frontline):
        new_frontline = set({})
        for p in frontline:
            for s in SHIFTS:
                x1 = p.x + s.x
                y1 = p.y + s.y
                what = _at(the_map, x1, y1)
                p1 = c.Point(x1, y1)
                if what == WALL or (p1 in result): continue
                d1 = _fast_shortest(the_map, p1, end)
                if d1 + distance > max_steps: continue
                result.add(p1)
                new_frontline.add(p1)
        distance += 1
        frontline = new_frontline
    return result


CACHE = {}


def _fast_shortest(the_map: [str], start, end: c.Point) -> int:
    key = (start, end)
    result = CACHE.get(key, -1)
    if result > -1: return result
    result = _shortest_distance(the_map, start, end)
    CACHE[key] = result
    return result


def _answer2(the_map: [str], max_desired_shortest: int) -> int:
    start = _find_location(the_map, START)
    end = _find_location(the_map, END)
    shortest = _shortest_distance(the_map, start, end)
    paths = _all_paths(the_map, start, end, shortest)
    count = 0
    for i, p1 in enumerate(paths):
        dist_to_end1 = _fast_shortest(the_map, p1, end)
        for j, p2 in enumerate(paths):
            if not (j > i): continue
            jump_distance = _distance_through_walls(p1, p2)
            if jump_distance > 20: continue
            dist_to_end2 = _fast_shortest(the_map, p2, end)
            if dist_to_end1 < dist_to_end2:
                # p1 is closer to end, we swap them
                p1, p2 = p2, p1
            ds1 = _fast_shortest(the_map, start, p1)
            d2e = _fast_shortest(the_map, p2, end)
            if (ds1 + d2e + jump_distance) <= max_desired_shortest:
                count += 1
    return count


class Day20(unittest.TestCase):

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
        # self.assertEqual(12 + 22 + 4 + 3, _answer2(the_map, max_desired_shortest - 70), "answer 2")

    def test_day(self):
        the_map = c.read_lines("20")
        start = _find_location(the_map, START)
        end = _find_location(the_map, END)
        shortest = _shortest_distance(the_map, start, end)
        max_desired_shortest = shortest - MIN_SAFE
        if not c.is_fast_only():
            # takes 39 minutes
            self.assertEqual(1415, _answer1(the_map, max_desired_shortest), "answer 1")
        # TODO answer 2 - 684977 - too low, took 38 minutes 17 seconds
        # self.assertEqual(0, _answer2(the_map, max_desired_shortest), "answer 2")
