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


def _area(region: Region) -> int:
    return len(region.points.points)  # object Points then its internal field points


def _price(region: Region) -> int:
    return _area(region) * region.perimeter  # area * perimeter


def _tx_point(p: Point) -> Point:
    # points are zero based integers for both x and y
    # with this transformation shifts will refer to boarders between points
    return Point(2 * p.x + 1, 2 * p.y + 1)


def _sides(points: [Point]) -> int:
    counter = Counter([b
                       for p in points
                       for b in _borders(_tx_point(p))])
    surface = [p for p in counter if counter[p] != 2]

    STEP = 2  # because of the _tx_point scaling we did earlier

    # sample 4 contains case when a line is not interrupted but de facto there are 2 angles:
    #  aBb
    #  BBB
    #  cBd
    def is_cross_of_two(surface: [], a: int, b: int, is_x_constant: bool, z: int) -> bool:
        middle = int((a + b) / 2)
        if is_x_constant:
            # x constant slice
            ap = Point(z + 1, middle)
            bp = Point(z - 1, middle)
        else:
            # y constant slice
            ap = Point(middle, z + 1)
            bp = Point(middle, z - 1)
        return any(i == ap for i in surface) and any(i == bp for i in surface)

    def count_sides_in_slice(surface: [], slice: [], is_x_constant: bool, z: int) -> int:
        result = 0
        slice.sort()
        prev = -100
        while len(slice):
            cur = slice[0]
            slice = slice[1:]
            if prev != cur - STEP or is_cross_of_two(surface, prev, cur, is_x_constant, z):
                result += 1
            prev = cur
        return result

    result = 0
    min_x = min(x for x, _ in surface)
    max_x = max(x for x, _ in surface)
    for cur_x in range(min_x, max_x + 1, STEP):
        slice = [y for x, y in surface if x == cur_x]
        result += count_sides_in_slice(surface, slice, True, cur_x)

    min_y = min(y for _, y in surface)
    max_y = max(y for _, y in surface)
    for cur_y in range(min_y, max_y + 1, STEP):
        slice = [x for x, y in surface if y == cur_y]
        result += count_sides_in_slice(surface, slice, False, cur_y)

    return result


_SHIFTS = [Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0)]


def _borders(p: Point) -> [Point]:
    return [Point(p.x + s.x, p.y + s.y) for s in _SHIFTS]


def _perimeter(points: [Point]) -> int:
    counter = Counter([b
                       for p in points
                       for b in _borders(_tx_point(p))])
    return sum(1 for v in counter.values() if v == 1)


def _get_region(p2p: {}, point: Point) -> {}:
    plant = p2p[point]
    points = {point}
    while True:

        neighbour_points = {neighbour
                            for cur_point in points
                            for neighbour in _borders(cur_point)
                            if p2p.get(neighbour) is not None and not any(x == neighbour for x in points) and p2p[
                                neighbour] == plant}
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


def _answer1(regions: [Region]) -> int:
    return sum(_price(r) for r in regions)


def _answer2(regions: [Region]) -> int:
    l = [(r.plant, _area(r), _sides(r.points.points)) for r in regions]
    return sum(_area(r) * _sides(r.points.points) for r in regions)


class Day12(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = c.read_lines(data)
        regions = _get_regions(the_map)
        self.assertEqual(a1, _answer1(regions), "answer 1")
        self.assertEqual(a2, _answer2(regions), "answer 2")

    def __test_answer2(self, data: str, a2: int):
        the_map = c.read_lines(data)
        regions = _get_regions(the_map)
        self.assertEqual(a2, _answer2(regions), "answer 2")

    def test_sample(self):
        self.__solution("12-1", 140, 80)

    def test_sample2(self):
        self.__test_answer2("12-2", 436)

    def test_sample3(self):
        self.__test_answer2("12-3", 236)

    def test_sample4(self):
        self.__test_answer2("12-4", 368)

    def test_sample5(self):
        self.__test_answer2("12-5", 1206)

    def test_day(self):
        self.__solution("12", 1433460, 855082)
