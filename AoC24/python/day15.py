import common as c
import unittest
from types import MappingProxyType
from typing import NamedTuple

BOX = "O"
ROBOT = "@"
SPACE = "."
WALL = "#"

GO_LEFT = "<"
GO_DOWN = "v"
GO_RIGHT = ">"
GO_UP = "^"


class Point(NamedTuple):
    x: int
    y: int


# Immutable map
TO_SHIFT = MappingProxyType({GO_LEFT: Point(-1, 0), GO_UP: Point(0, -1), GO_DOWN: Point(0, 1), GO_RIGHT: Point(1, 0)})


def _gps(p: Point) -> int:
    return p.x + (100 * p.y)


#
def _parse(data: [str]) -> ([str], str):
    """Returns the map and the list of instructions to move robot."""
    the_map = []
    navigation = ""
    is_map = True
    for line in data:
        if len(line) <= 1:
            is_map = False
            continue
        if is_map:
            the_map.append(line)
        else:
            navigation += line
    return the_map, navigation


def _is_in(the_map: [str], p: Point) -> bool:
    x, y = p
    return x >= 0 and y >= 0 and y < len(the_map) and x < len(the_map[0])


def _may_move(the_map: [str], robot: Point, shift: Point) -> bool:
    """Checking if the move is possible."""
    rx, ry = robot
    sx, sy = shift
    spx = rx + sx
    spy = ry + sy
    sp = Point(spx, spy)
    if not _is_in(the_map, sp):
        return False  # we can not go out of the map
    what = the_map[spy][spx]
    if what == SPACE:
        return True
    if what == BOX:
        return _may_move(the_map, sp, shift)
    return False  # the remaining case is WALL and we cannot move walls


def _move(the_map: [str], robot: Point, shift: Point) -> bool:
    """Move the robot and the impacted boxes"""
    return False  # TODO - implement


def _navigate(the_map: [str], navigation: str):
    robot_x, robot_y = [(line.find(ROBOT), y) for y, line in enumerate(the_map) if line.find(ROBOT) != -1][0]
    robot = Point(robot_x, robot_y)
    for step in navigation:
        shift = TO_SHIFT[step]
        if _may_move(the_map, robot, shift):
            _move(the_map, robot, shift)


def _answer1(the_map: [str], navigation: str) -> int:
    _navigate(the_map, navigation)
    return sum(_gps(Point(x, y))
               for y, line in enumerate(the_map)
               for x, what in enumerate(line)
               if what == BOX)


def _answer2(the_map: [str], navigation: str) -> int:
    return 0


class Day15(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map, navigation = _parse(c.read_lines(data))
        self.assertEqual(a1, _answer1(the_map, navigation), "answer 1")
        # ATTENTION the_map can be changed at this point!
        # self.assertEqual(a2, _answer2(the_map, navigation), "answer 2")

    def test_sample(self):
        self.__solution("15-2", 2028, 0)

    def test_larger_sample(self):
        self.__solution("15-1", 10092, 0)

    def test_day(self):
        self.__solution("15", 0, 0)
