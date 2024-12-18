import common as c
import unittest
from types import MappingProxyType
from common import Point

BOX = "O"
ROBOT = "@"
SPACE = "."
WALL = "#"
BOX2 = "[]"
BOX2L = "["
BOX2R = "]"

GO_LEFT = "<"
GO_DOWN = "v"
GO_RIGHT = ">"
GO_UP = "^"

# Immutable map
TO_SHIFT = MappingProxyType({GO_LEFT: Point(-1, 0), GO_UP: Point(0, -1), GO_DOWN: Point(0, 1), GO_RIGHT: Point(1, 0)})


def _gps(p: Point) -> int:
    return p.x + (100 * p.y)


def _sum_gps(the_map: [str], box: str) -> int:
    return sum(_gps(Point(x, y))
               for y, line in enumerate(the_map)
               for x, what in enumerate(line)
               if what == box)


def _parse(data: [str]) -> ([str], str):
    """Returns the map and the list of instructions to move robot."""
    the_map = []
    navigation = ""
    is_map = True
    for line in data:
        if len(line) == 0:
            is_map = False
            continue
        if is_map:
            the_map.append(line)
        else:
            navigation += line
    return the_map, navigation


def _get(the_map: [str], point: Point) -> str:
    x, y = point
    return the_map[y][x]


def _set(the_map: [str], point: Point, what: str):
    x, y = point
    line = the_map[y]
    new_line = line[:x] + what + line[x + 1:]
    the_map[y] = new_line


def _may_move(the_map: [str], robot: Point, shift: Point) -> bool:
    """Checking if the move is possible."""
    rx, ry = robot
    sx, sy = shift
    sp = Point(rx + sx, ry + sy)
    what = _get(the_map, sp)
    if what == SPACE:
        return True
    if what == BOX or what == BOX2L or what == BOX2R:
        return _may_move(the_map, sp, shift)
    return False  # the remaining case is WALL and we cannot move walls


def _move(the_map: [str], robot: Point, shift: Point, what: str) -> Point:
    """Move the robot and the impacted boxes and returns new position"""
    rx, ry = robot
    sx, sy = shift
    sp = Point(rx + sx, ry + sy)
    what_at_dist = _get(the_map, sp)
    if what_at_dist == SPACE:
        _set(the_map, robot, SPACE)
        _set(the_map, sp, what)
    elif what_at_dist == BOX or what_at_dist == BOX2L or what_at_dist == BOX2R:
        _move(the_map, sp, shift, what_at_dist)
        _set(the_map, robot, SPACE)
        _set(the_map, sp, what)
    return sp


def _navigate(the_map: [str], navigation: str):
    robot_x, robot_y = [(line.find(ROBOT), y) for y, line in enumerate(the_map) if line.find(ROBOT) != -1][0]
    robot = Point(robot_x, robot_y)
    for step in navigation:
        shift = TO_SHIFT[step]
        if _may_move(the_map, robot, shift):
            robot = _move(the_map, robot, shift, ROBOT)


def _answer1(the_map: [str], navigation: str) -> int:
    _navigate(the_map, navigation)
    return _sum_gps(the_map, BOX)


def _wider_map(the_map: [str]) -> [str]:
    m = {WALL: WALL + WALL, BOX: BOX2, SPACE: SPACE + SPACE, ROBOT: ROBOT + SPACE}
    new_line = lambda line: "".join([m[what] for what in line])
    return [new_line(line) for line in the_map]


def _navigate2(the_map: [str], navigation: str):
    robot_x, robot_y = [(line.find(ROBOT), y) for y, line in enumerate(the_map) if line.find(ROBOT) != -1][0]
    robot = Point(robot_x, robot_y)
    for step in navigation:
        shift = TO_SHIFT[step]
        if _may_move2(the_map, robot, shift):
            robot = _move2(the_map, robot, shift, ROBOT)


def _may_move2(the_map: [str], robot: Point, shift: Point) -> bool:
    """Checking if the move is possible."""
    sx, sy = shift
    if sy == 0:
        # shift moving on horizon axis remains the same
        return _may_move(the_map, robot, shift)

    rx, ry = robot
    sp = Point(rx + sx, ry + sy)
    what = _get(the_map, sp)
    if what == SPACE:
        return True
    if what == BOX2L:
        return _may_move2(the_map, sp, shift) and _may_move2(the_map, Point(sp.x + 1, sp.y), shift)
    if what == BOX2R:
        return _may_move2(the_map, sp, shift) and _may_move2(the_map, Point(sp.x - 1, sp.y), shift)
    return False  # the remaining case is WALL and we cannot move walls


def _move2(the_map: [str], robot: Point, shift: Point, what: str) -> Point:
    sx, sy = shift
    if sy == 0:
        # shift moving on horizon axis remains the same
        return _move(the_map, robot, shift, what)

    rx, ry = robot
    sp = Point(rx + sx, ry + sy)
    what_at_dist = _get(the_map, sp)
    if what_at_dist == SPACE:
        _set(the_map, robot, SPACE)
        _set(the_map, sp, what)

    elif what_at_dist == BOX2L or what_at_dist == BOX2R:
        _move2(the_map, sp, shift, what_at_dist)
        if what_at_dist == BOX2L:
            sp_pair = Point(sp.x + 1, sp.y)
            what_pair = BOX2R
        else:
            sp_pair = Point(sp.x - 1, sp.y)
            what_pair = BOX2L
        _move2(the_map, sp_pair, shift, what_pair)
        _set(the_map, robot, SPACE)
        _set(the_map, sp, what)
    return sp


def _answer2(the_map: [str], navigation: str) -> int:
    _navigate2(the_map, navigation)
    return _sum_gps(the_map, BOX2L)


class Day15(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map, navigation = _parse(c.read_lines(data))
        wider_map = _wider_map(the_map)
        self.assertEqual(a1, _answer1(the_map, navigation), "answer 1")
        self.assertEqual(a2, _answer2(wider_map, navigation), "answer 2")

    def test_sample(self):
        the_map, navigation = _parse(c.read_lines("15-2"))
        self.assertEqual(2028, _answer1(the_map, navigation), "answer 1 sample")

    def test_larger_sample(self):
        self.__solution("15-1", 10092, 9021)

    def test_bug_cascading_move(self):
        the_map, navigation = _parse(c.read_lines("15-3"))
        the_map_ok = c.read_lines("15-3a")
        _navigate2(the_map, navigation)
        self.assertEqual(the_map_ok, the_map, "bug cascading move")

    def test_day(self):
        self.__solution("15", 1294459, 1319212)
