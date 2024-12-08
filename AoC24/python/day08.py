import common as c
import unittest

EMPTY = '.'


def _all_frequencies(the_map: [str]) -> {}:
    return {c for l in the_map
            for c in l
            if c != EMPTY}


# TODO - early draft not sure it works in all a, b positions
def _antinodes_limitless(a: (int, int), b: (int, int)) -> [(int, int)]:
    ''' May produce antinodes outside the map. '''
    ax, ay = a
    bx, by = b
    dx = ax - bx
    dy = ay - by
    return [(ax + dx, ay + dy), (bx - dx, by - dy)]


def _antennas_positions(the_map: [str], frequency: str) -> [(int, int)]:
    return [(x, y) for y in range(len(the_map))
            for x in range(len(the_map[0]))
            if the_map[y][x] == frequency]


def _answer1(the_map: [str]) -> int:
    frequencies = _all_frequencies(the_map)
    all_antinodes = set()
    for f in frequencies:
        # all antennas of the given frequency
        antennas = _antennas_positions(the_map, f)
        for i, a in enumerate(antennas):
            for j, b in enumerate(antennas):
                if i < j:
                    ij_antinodes = _antinodes_limitless(a, b)
                    all_antinodes.update(ij_antinodes)

    y_n = len(the_map)
    x_n = len(the_map[0])
    result = 0
    for a in all_antinodes:
        x, y = a
        if (0 <= x) and (x < x_n) and (0 <= y) and (y < y_n):
            result += 1
    return result


def _answer2(the_map: [str]) -> int:
    return 0


class Day08(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = c.read_lines(data)
        self.assertEqual(_answer1(the_map), a1, "answer 1")
        self.assertEqual(_answer2(the_map), a2, "answer 2")

    def test_sample(self):
        self.__solution("08-1", 14, 0)

    def test_day(self):
        self.__solution("08", 269, 0)
