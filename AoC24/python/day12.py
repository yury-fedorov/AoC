import common as c
import unittest
from typing import NamedTuple
import collections
from collections import Counter


class Point(NamedTuple):
    x: int
    y: int


Points = collections.namedtuple('Points', ['points'])


class Region(NamedTuple):
    plant: str
    points: Points  # len(points) - area
    perimeter: int


def _price(region: Region) -> int:
    return len(region.points) * region.perimeter  # area * perimeter


SHIFTS = [Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0)]


def _perimeter(points: [Point]) -> int:
    # points are zero based integers for both x and y
    def tx(z: int) -> int:
        return (2 * z) + 1

    def tx_point(p: Point) -> Point:
        return Point(tx(p.x), tx(p.y))

    def borders(p: Point) -> [Point]:
        return [Point(p.x + s.x, p.y + s.y) for s in SHIFTS]

    counter = Counter([b
                       for p in points
                       for b in borders(tx_point(p))])

    return sum(1 for v in counter.values() if v == 1)


def _get_regions(the_map: []) -> []:
    result = []
    x = 0
    y = 0
    plant = the_map[y][x]
    points = [Point(x, y)]
    result.append(Region(plant, Points(points), _perimeter(points)))
    # TODO implement!
    return result


def _answer1(the_map: []) -> int:
    regions = _get_regions(the_map)
    return sum(_price(r) for r in regions)


def _answer2(the_map: []) -> int:
    return 0


class Day12(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = c.read_lines(data)
        self.assertEqual(a1, _answer1(the_map), "answer 1")
        self.assertEqual(a2, _answer2(the_map), "answer 2")

    def test_sample(self):
        self.__solution("12-1", 140, 0)

    def test_day(self):
        self.__solution("12", 0, 0)
