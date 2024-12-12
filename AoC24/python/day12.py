import common as c
import unittest
from typing import NamedTuple
from collections import namedtuple
from collections import Counter


class Point(NamedTuple):
    x: int
    y: int


Points = namedtuple('Points', ['points'])


class Region(NamedTuple):
    plant: str
    points: Points  # len(points) - area
    perimeter: int


def _price(region: Region) -> int:
    return len(region.points) * region.perimeter  # area * perimeter


_SHIFTS = [Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0)]


def _borders(p: Point) -> [Point]:
    return [Point(p.x + s.x, p.y + s.y) for s in _SHIFTS]


def _perimeter(points: [Point]) -> int:
    # points are zero based integers for both x and y
    def tx(z: int) -> int:
        return (2 * z) + 1

    def tx_point(p: Point) -> Point:
        return Point(tx(p.x), tx(p.y))

    counter = Counter([b
                       for p in points
                       for b in _borders(tx_point(p))])

    return sum(1 for v in counter.values() if v == 1)


def _get_region(p2p: {}, point: Point) -> {}:
    plant = p2p[point]
    points = {point}
    while True:
        neighbours = {neighbour
                      for cur_point in points
                      for neighbour in _borders(cur_point)
                      if p2p.get(neighbour) is not None
                      }
        neighbour_points = set([neighbour
                                for neighbour in neighbours
                                if not any(x == neighbour for x in points) and p2p[
                                    neighbour] == plant])
        if len(neighbour_points) == 0:
            break
        points.update(neighbour_points)
    return points


def _get_regions(the_map: []) -> []:
    # point to plant type
    pairs = [(Point(x, y), plant)
             for y, line in enumerate(the_map)
             for x, plant in enumerate(line)]
    p2p = dict({p for p in pairs})
    pending_processing = {x for x in p2p}
    result = []
    while len(pending_processing) > 0:
        point = pending_processing.pop()
        plant = p2p[point]
        points = _get_region(p2p, point)
        pending_processing.difference_update(points)  # remove from pending all points of the region
        result.append(Region(plant, Points(points), _perimeter(points)))
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
