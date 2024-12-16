import common as c
import sys
import unittest
from enum import Enum
from typing import NamedTuple


class Point(NamedTuple):
    x: int
    y: int


class Direction(Enum):
    EAST = Point(1, 0)
    NORTH = Point(0, -1)
    WEST = Point(-1, 0)
    SOUTH = Point(0, 1)


DIRECTIONS = (Direction.EAST, Direction.NORTH, Direction.WEST, Direction.SOUTH)  # read only

TURN90_SCORE = 1000


class What(Enum):
    START = "S"
    END = "E"
    WALL = "#"
    SPACE = "."


class Position(NamedTuple):
    location: Point
    direction: Direction


def _find_point(the_map: [str], what: What) -> Point:
    return [Point(line.find(what.value), y) for y, line in enumerate(the_map) if line.find(what.value) != -1][0]


def _at(the_map: [str], p: Point) -> str:
    return the_map[p.y][p.x]


def _opposite(d: Direction) -> Direction:
    index = DIRECTIONS.index(d)
    return DIRECTIONS[(index + 2) % 4]


def _directions(the_map: [str], p: Position) -> [Direction]:
    opposite_direction: Direction = _opposite(p.direction)
    return [d for d in DIRECTIONS
            if _at(the_map, Point(p.location.x + d.value.x,
                                  p.location.y + d.value.y)) != What.WALL.value and d != opposite_direction]


def _score_step(previous_position: Position, new_direction: Direction) -> int:
    return 1 if previous_position.direction == new_direction else TURN90_SCORE


def _min_score(the_map: [str], score_map: {Position: int}) -> int:
    starting_position = Position(_find_point(the_map, What.START), Direction.EAST)
    positions = {starting_position}
    while len(positions) > 0:
        cur_pos = positions.pop()
        dirs = _directions(the_map, cur_pos)
        prev_score = score_map[cur_pos]
        for new_dir in dirs:
            delta_score = _score_step(cur_pos, new_dir)
            if delta_score == 1:
                new_loc = Point(cur_pos.location.x + new_dir.value.x, cur_pos.location.y + new_dir.value.y)
            else:
                new_loc = cur_pos.location
            new_pos = Position(new_loc, new_dir)
            new_score_in_cache = score_map.get(new_pos, sys.maxsize)
            new_score = prev_score + delta_score
            if new_score < new_score_in_cache:
                score_map[new_pos] = new_score
                positions.add(new_pos)

    end_position = _find_point(the_map, What.END)
    return min([score_map.get(Position(end_position, d), sys.maxsize) for d in DIRECTIONS])


def _answer1(the_map: [str]) -> int:
    starting_position = Position(_find_point(the_map, What.START), Direction.EAST)
    score_map = {starting_position: 0}
    return _min_score(the_map, score_map)


def _min_path(the_map: [str], score_map: {Position: int}, cur_pos: Position, prev_score: int, score_limit: int) -> {
    Point}:
    end_point = _find_point(the_map, What.END)
    if cur_pos.location == end_point:
        # happy end, we are already at the destination
        return {end_point}

    result = set({})
    dirs = _directions(the_map, cur_pos)
    for new_dir in dirs:
        delta_score = _score_step(cur_pos, new_dir)
        new_score = prev_score + delta_score
        if new_score > score_limit:
            continue

        if delta_score == 1:
            new_loc = Point(cur_pos.location.x + new_dir.value.x, cur_pos.location.y + new_dir.value.y)
        else:
            new_loc = cur_pos.location
        new_pos = Position(new_loc, new_dir)

        best_score_at_position = score_map[new_pos]
        if best_score_at_position != new_score:
            continue

        tail = _min_path(the_map, score_map, new_pos, new_score, score_limit)
        if len(tail):
            result.add(new_loc)
            result.update(tail)

    return result


def _answer2(the_map: [str]) -> int:
    starting_position = Position(_find_point(the_map, What.START), Direction.EAST)
    score_map = {starting_position: 0}
    score_limit = _min_score(the_map, score_map)
    return len(_min_path(the_map, score_map, starting_position, 0, score_limit))


class Day16(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = c.read_lines(data)
        self.assertEqual(a1, _answer1(the_map), "answer 1")
        self.assertEqual(a2, _answer2(the_map), "answer 2")

    def test_sample(self):
        self.__solution("16-1", 7036, 45)

    def test_sample2(self):
        self.__solution("16-2", 11048, 64)

    def test_day(self):
        self.__solution("16", 105508, 0)
