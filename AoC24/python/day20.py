import common as c
import unittest

START = 'S'
END = 'E'
WALL = '#'
TRACK = '.'

MIN_SAFE = 100  # for answer 1

SHIFTS = [c.Point(1, 0), c.Point(0, 1), c.Point(-1, 0), c.Point(0, -1)]


def _find_location(the_map: [str], what: str) -> c.Point:
    for y, line in enumerate(the_map):
        x = line.find(what)
        if x >= 0: return c.Point(x, y)
    raise ValueError()


def _get_track_path(the_map: [str], start: c.Point, end: c.Point) -> dict[c.Point, int]:
    dist = {start: 0}
    curr = start
    while curr != end:
        for s in SHIFTS:
            nxt = c.Point(curr.x + s.x, curr.y + s.y)
            if 0 <= nxt.y < len(the_map) and 0 <= nxt.x < len(the_map[0]):
                if the_map[nxt.y][nxt.x] != WALL and nxt not in dist:
                    dist[nxt] = dist[curr] + 1
                    curr = nxt
                    break
    return dist


def _shortest_distance(the_map: [str], start: c.Point, end: c.Point, max_distance: int | None = None) -> int | None:
    dist = _get_track_path(the_map, start, end)
    return dist.get(end, None)


def _count_cheats(the_map: [str], max_cheat_length: int, min_save: int) -> int:
    start = _find_location(the_map, START)
    end = _find_location(the_map, END)
    dist = _get_track_path(the_map, start, end)

    count = 0
    for p1, d1 in dist.items():
        for dx in range(-max_cheat_length, max_cheat_length + 1):
            rem = max_cheat_length - abs(dx)
            for dy in range(-rem, rem + 1):
                if dx == 0 and dy == 0:
                    continue
                d = abs(dx) + abs(dy)
                p2 = c.Point(p1.x + dx, p1.y + dy)
                if p2 in dist:
                    d2 = dist[p2]
                    if d2 - d1 - d >= min_save:
                        count += 1
    return count


def _answer1(the_map: [str], max_desired_shortest: int) -> int:
    start = _find_location(the_map, START)
    end = _find_location(the_map, END)
    shortest = _shortest_distance(the_map, start, end)
    min_save = shortest - max_desired_shortest
    return _count_cheats(the_map, 2, min_save)


def _answer2(the_map: [str], max_cheat_length: int, min_save: int) -> int:
    return _count_cheats(the_map, max_cheat_length, min_save)


class Day20(unittest.TestCase):

    def test_shortest(self):
        the_map = c.read_lines("20-1")
        start = _find_location(the_map, START)
        end = _find_location(the_map, END)
        self.assertEqual(84, _shortest_distance(the_map, start, end), "shortest")

    def test_sample(self):
        the_map = c.read_lines("20-1")
        start = _find_location(the_map, START)
        end = _find_location(the_map, END)
        shortest = _shortest_distance(the_map, start, end)
        max_desired_shortest = shortest - 20
        self.assertEqual(5, _answer1(the_map, max_desired_shortest), "answer 1")
        self.assertEqual(3, _answer2(the_map, 20, 76), "answer 2 (>=76)")
        self.assertEqual(285, _answer2(the_map, 20, 50), "answer 2 (>=50)")

    def test_day(self):
        the_map = c.read_lines("20")
        start = _find_location(the_map, START)
        end = _find_location(the_map, END)
        shortest = _shortest_distance(the_map, start, end)
        max_desired_shortest = shortest - MIN_SAFE
        self.assertEqual(1415, _answer1(the_map, max_desired_shortest), "answer 1")
        self.assertEqual(1022577, _answer2(the_map, 20, 100), "answer 2")


if __name__ == '__main__':
    unittest.main()
