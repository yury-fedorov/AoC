import common as c
import unittest


def _all_trailheads(the_map: []) -> []:
    """All valid trailhead positions/0 positions."""
    return [(ix, iy) for iy, line in enumerate(the_map)
            for ix, pos in enumerate(line)
            if pos == '0']


SHIFTS = [(1, 0), (-1, 0), (0, 1), (0, -1)]


def _score_trailhead(the_map: [], trailhead: (int, int), is_part1: bool) -> int:
    """Scoring function for both answers"""
    n_y, n_x = len(the_map), len(the_map[0])

    def is_in(p: (int, int)) -> bool:
        x, y = p
        return 0 <= x < n_x and 0 <= y < n_y

    positions = {trailhead} if is_part1 else [trailhead]
    for cur_level in range(9):
        next_level = str(cur_level + 1)
        next_positions = set() if is_part1 else []
        for p in positions:
            x0, y0 = p
            for s in SHIFTS:
                dx, dy = s
                x1 = x0 + dx
                y1 = y0 + dy
                p1 = x1, y1
                if is_in(p1) and the_map[y1][x1] == next_level:
                    if is_part1:
                        next_positions.add(p1)
                    else:
                        next_positions.append(p1)
        positions = next_positions
    return len(positions)


def _answer1(the_map: []) -> int:
    trailheads = _all_trailheads(the_map)
    return sum(_score_trailhead(the_map, p, True) for p in trailheads)


def _answer2(the_map: []) -> int:
    trailheads = _all_trailheads(the_map)
    return sum(_score_trailhead(the_map, p, False) for p in trailheads)


class Day10(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        the_map = c.read_lines(data)
        self.assertEqual(a1, _answer1(the_map), "answer 1")
        self.assertEqual(a2, _answer2(the_map), "answer 2")

    def test_sample(self):
        self.__solution("10-1", 1, 16)

    def test_sample2(self):
        self.__solution("10-2", 36, 81)

    def test_day(self):
        self.__solution("10", 496, 1120)
