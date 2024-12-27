import common as c
import unittest

EMPTY = '.'


def _all_frequencies(the_map: [str]) -> {}:
    return {c for l in the_map for c in l if c != EMPTY}


def _antinodes(a: (int, int), b: (int, int), map_size: (int, int), is_repeated: bool) -> [(int, int)]:
    ax, ay = a
    bx, by = b
    dx = ax - bx
    dy = ay - by

    x_n, y_n = map_size

    def is_in(p: (int, int)) -> bool:
        x, y = p
        return (0 <= x) and (x < x_n) and (0 <= y) and (y < y_n)

    result = []
    # for repeated full map coverage, otherwise one iteration: range(1,2)
    r = range(0, max(x_n, y_n)) if is_repeated else range(1, 2)
    for i in r:
        ai = (ax + (i * dx), ay + (i * dy))
        bi = (bx - (i * dx), by - (i * dy))
        valid_cases = [xi for xi in [ai, bi] if is_in(xi)]
        result.extend(valid_cases)
        if len(valid_cases) == 0:
            break

    return [e for e in result if is_in(e)]


def _antennas_positions(the_map: [str], frequency: str) -> [(int, int)]:
    return [(x, y) for y in range(len(the_map))
            for x in range(len(the_map[0]))
            if the_map[y][x] == frequency]


def _answer(the_map: [str], is_repeated: bool) -> int:
    map_size = (len(the_map[0]), len(the_map))
    frequencies = _all_frequencies(the_map)
    all_antinodes = set()
    for f in frequencies:
        # all antennas of the given frequency
        antennas = _antennas_positions(the_map, f)
        for i, a in enumerate(antennas):
            for j, b in enumerate(antennas):
                if i < j:
                    all_antinodes.update(_antinodes(a, b, map_size, is_repeated))
    return len(all_antinodes)


def _answer1(the_map: [str]) -> int: return _answer(the_map, False)


def _answer2(the_map: [str]) -> int: return _answer(the_map, True)


class Day08(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = c.read_lines(data)
        self.assertEqual(_answer1(the_map), a1, "answer 1")
        self.assertEqual(_answer2(the_map), a2, "answer 2")

    def test_sample(self):
        self.__solution("08-1", 14, 34)

    def test_day(self):
        self.__solution("08", 269, 949)
