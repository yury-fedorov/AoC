import common as c
from functools import cache
import unittest

A = 'A'
UP = '^'
DOWN = 'v'
LEFT = '<'
RIGHT = '>'

NUM_COORDS = {
    '7': (0, 0), '8': (1, 0), '9': (2, 0),
    '4': (0, 1), '5': (1, 1), '6': (2, 1),
    '1': (0, 2), '2': (1, 2), '3': (2, 2),
    '0': (1, 3), 'A': (2, 3)
}
NUM_GAP = (0, 3)

DIR_COORDS = {
    '^': (1, 0), 'A': (2, 0),
    '<': (0, 1), 'v': (1, 1), '>': (2, 1)
}
DIR_GAP = (0, 0)


def _get_keypad_paths(start: str, end: str, pos_map: dict, gap: tuple[int, int]) -> list[str]:
    x1, y1 = pos_map[start]
    x2, y2 = pos_map[end]
    dx = x2 - x1
    dy = y2 - y1

    horiz = ('>' * dx) if dx > 0 else ('<' * (-dx))
    vert = ('v' * dy) if dy > 0 else ('^' * (-dy))

    paths = []
    if (x2, y1) != gap:
        paths.append(horiz + vert + 'A')
    if (x1, y2) != gap and horiz != "" and vert != "":
        paths.append(vert + horiz + 'A')

    return paths


@cache
def _dir_cost(seq: str, depth: int) -> int:
    if depth == 0:
        return len(seq)

    total = 0
    curr = 'A'
    for ch in seq:
        paths = _get_keypad_paths(curr, ch, DIR_COORDS, DIR_GAP)
        total += min(_dir_cost(p, depth - 1) for p in paths)
        curr = ch
    return total


def _shortest_sequence_len(code: str, robots: int) -> int:
    total = 0
    curr = 'A'
    for ch in code:
        paths = _get_keypad_paths(curr, ch, NUM_COORDS, NUM_GAP)
        total += min(_dir_cost(p, robots) for p in paths)
        curr = ch
    return total


def _complexity(code: str, length: int) -> int:
    return int(code.rstrip('A')) * length


def _answer1(codes: [str]) -> int:
    return sum(_complexity(code, _shortest_sequence_len(code, 2)) for code in codes)


def _answer2(codes: [str]) -> int:
    return sum(_complexity(code, _shortest_sequence_len(code, 25)) for code in codes)


class Day21(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        codes = c.read_lines(data)
        self.assertEqual(a1, _answer1(codes), "answer 1")
        self.assertEqual(a2, _answer2(codes), "answer 2")

    def test_complexity(self):
        seq = "<vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A"
        self.assertEqual(68 * 29, _complexity("029A", len(seq)), "complexity")
        self.assertEqual(68, _shortest_sequence_len("029A", 2), "shortest sequence 2 - 029A")
        self.assertEqual(60, _shortest_sequence_len("980A", 2), "shortest sequence 2 - 980A")
        self.assertEqual(68, _shortest_sequence_len("179A", 2), "shortest sequence 2 - 179A")
        self.assertEqual(64, _shortest_sequence_len("456A", 2), "shortest sequence 2 - 456A")
        self.assertEqual(64, _shortest_sequence_len("379A", 2), "shortest sequence 2 - 379A")

    def test_sample(self):
        codes = c.read_lines("21-1")
        self.assertEqual(126384, _answer1(codes), "answer 1")
        self.assertEqual(154115708116294, _answer2(codes), "answer 2 sample")

    def test_day(self):
        self.__solution("21", 156714, 191139369248202)


if __name__ == '__main__':
    unittest.main()
