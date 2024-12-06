import copy
import common as c
import unittest


def _start(the_map: []) -> (int, int):
    for y, line in enumerate(the_map):
        x = line.find('^')
        if x != -1:
            return x, y


_DIRECTIONS = [(0, -1), (1, 0), (0, 1), (-1, 0)]
_WALL = '#'


def _rotate(direction: (int, int)) -> (int, int):
    index = _DIRECTIONS.index(direction)
    return _DIRECTIONS[(index + 1) % len(_DIRECTIONS)]


def _is_wall(material: str) -> bool:
    return material == _WALL


def _navigate(the_map: [], start_point: (int, int)) -> (int, bool):
    '''Path length und if we went out or not'''
    xmax = len(the_map[0])
    ymax = len(the_map)

    def is_in(p: (int, int)) -> bool:
        x, y = p
        return False if x < 0 or y < 0 or x >= xmax or y >= ymax else True

    path = {start_point}
    p = start_point
    d = _DIRECTIONS[0]
    states = {(p, d)}

    while True:
        x0, y0 = p
        dx, dy = d
        p1 = x0 + dx, y0 + dy
        if not is_in(p1):
            return len(path), True
        x1, y1 = p1
        if _is_wall(the_map[y1][x1]):
            d = _rotate(d)
        else:
            p = p1
            path.add(p)

        s = (p, d)
        if s in states:
            return len(path), False
        states.add(s)


def _answer1(the_map: [], start_point: (int, int)) -> int:
    path_length, _ = _navigate(the_map, start_point)
    return path_length


def _is_loop(the_map: [], start_point: (int, int)) -> bool:
    _, is_out = _navigate(the_map, start_point)
    return not is_out


def _answer2(the_map: [], start_point: (int, int)) -> int:
    xmax = len(the_map[0])
    ymax = len(the_map)
    result = 0
    for x in range(xmax):
        for y in range(ymax):
            new_map = copy.deepcopy(the_map)
            if not _is_wall(new_map[y][x]):
                new_map[y] = new_map[y][:x] + '#' + new_map[y][x + 1:]
                _, is_out = _navigate(new_map, start_point)
                result += 0 if is_out else 1
    return result


class Day06(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = c.read_lines(data)
        start_point = _start(the_map)
        self.assertEqual(_answer1(the_map, start_point), a1, "answer 1")
        if not c.is_fast_only():  # takes 107 seconds
            self.assertEqual(_answer2(the_map, start_point), a2, "answer 2")

    def test_sample(self):
        self.__solution("06-1", 41, 6)

    def test_day(self):
        self.__solution("06", 5331, 1812)
