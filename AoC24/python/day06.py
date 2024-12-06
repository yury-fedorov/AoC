import common as c
import unittest


def _read_input(data: str):
    with c.open_file(data) as f:
        return [line.strip() for line in f]


def _start(the_map: []) -> (int, int):
    for y, line in enumerate(the_map):
        x = line.find('^')
        if x != -1:
            return x, y


_DIRECTIONS = [(0, -1), (1, 0), (0, 1), (-1, 0)]


def _rotate(direction: (int, int)) -> (int, int):
    index = _DIRECTIONS.index(direction)
    return _DIRECTIONS[(index + 1) % len(_DIRECTIONS)]


def _is_wall(material: str) -> bool:
    return material == '#'


def _answer1(the_map: [], start_point: (int, int)) -> int:
    xmax = len(the_map[0])
    ymax = len(the_map)

    def is_in(p: (int, int)) -> bool:
        x, y = p
        return False if x < 0 or y < 0 or x >= xmax or y >= ymax else True

    path = {start_point}
    p = start_point
    d = _DIRECTIONS[0]

    while is_in(p):
        x0, y0 = p
        dx, dy = d
        p1 = x0 + dx, y0 + dy
        if not is_in(p1):
            return len(path)
        x1, y1 = p1
        if _is_wall(the_map[y1][x1]):
            d = _rotate(d)
        else:
            p = p1
            path.add(p)


def _answer2(the_map: []) -> int:
    return 0


class Day06(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = _read_input(data)
        start_point = _start(the_map)
        self.assertEqual(_answer1(the_map, start_point), a1, "answer 1")
        # self.assertEqual(_answer2(the_map), a2, "answer 2")

    def test_sample(self):
        self.__solution("06-1", 41, 0)

    def test_day(self):
        self.__solution("06", 5331, 0)
